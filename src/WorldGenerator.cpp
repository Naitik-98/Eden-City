#include "WorldGenerator.h"

#define STB_PERLIN_IMPLEMENTATION
#include "../lib/stb/stb_perlin.h"

void WorldGenerator::generateFlatWorld(World* world) {
    if (!world) return;

    int maxWorldX = Config::WORLD_CHUNKS_X * Config::CHUNK_SIZE;
    int maxWorldY = Config::WORLD_CHUNKS_Y * Config::CHUNK_SIZE;
    int maxWorldZ = Config::WORLD_CHUNKS_Z * Config::CHUNK_SIZE;

    for (int x = 0; x < maxWorldX; ++x) {
        for (int z = 0; z < maxWorldZ; ++z) {
            // Ground level is at half the world height
            int groundHeight = maxWorldY / 2;

            for (int y = 0; y < maxWorldY; ++y) {
                if (y < groundHeight - 3) {
                    world->setBlockAt(x, y, z, BlockType::STONE, true);
                } else if (y < groundHeight) {
                    world->setBlockAt(x, y, z, BlockType::DIRT, true);
                } else if (y == groundHeight) {
                    world->setBlockAt(x, y, z, BlockType::GRASS, true);
                } else {
                    world->setBlockAt(x, y, z, BlockType::AIR, false);
                }
            }
        }
    }
}

void WorldGenerator::generateEdenCity(World* world) {
    if (!world) return;

    int maxWorldX = Config::WORLD_CHUNKS_X * Config::CHUNK_SIZE;
    int maxWorldY = Config::WORLD_CHUNKS_Y * Config::CHUNK_SIZE;
    int maxWorldZ = Config::WORLD_CHUNKS_Z * Config::CHUNK_SIZE;

    // We will generate the world based on 3D noise.
    // If noise density is > threshold, we place a solid block.
    // We add a vertical gradient to bias generation towards the middle Y area,
    // creating distinct floating islands.

    for (int x = 0; x < maxWorldX; ++x) {
        for (int z = 0; z < maxWorldZ; ++z) {
            for (int y = 0; y < maxWorldY; ++y) {
                
                // Scale coordinates for noise function (frequency)
                float nx = static_cast<float>(x) * 0.05f;
                float ny = static_cast<float>(y) * 0.08f; // squish Y to make islands flatter
                float nz = static_cast<float>(z) * 0.05f;

                // Base 3D noise [-1.0, 1.0]
                float noiseVal = stb_perlin_noise3(nx, ny, nz, 0, 0, 0);

                // Vertical gradient: highest density in the middle, tapers to 0 at top/bottom
                float midY = maxWorldY / 2.0f;
                float distFromMid = std::abs(y - midY);
                // normalized dist [0, 1] relative to half the world
                float normDist = distFromMid / midY; 
                
                // Density formula: noise minus height penalty
                // Penalty is stronger the further from center we are
                float density = noiseVal - (normDist * 1.5f);

                if (density > 0.0f) {
                    world->setBlockAt(x, y, z, BlockType::DIRT, true);
                } else {
                    world->setBlockAt(x, y, z, BlockType::AIR, false);
                }
            }
        }
    }

    // Second pass: surface dressing (Grass on top of Dirt, Stone below Dirt)
    for (int x = 0; x < maxWorldX; ++x) {
        for (int z = 0; z < maxWorldZ; ++z) {
            for (int y = 0; y < maxWorldY; ++y) {
                Block b = world->getBlock(x, y, z);
                if (b.type == BlockType::DIRT && b.active) {
                    // Check block above
                    Block above = world->getBlock(x, y + 1, z);
                    if (!above.active || above.type == BlockType::AIR) {
                        // Exposed to air -> Grass
                        world->setBlockAt(x, y, z, BlockType::GRASS, true);
                    } else {
                        // Check block deeply below to convert to stone
                        Block below1 = world->getBlock(x, y - 1, z);
                        Block below2 = world->getBlock(x, y - 2, z);
                        if (below1.active && below2.active) {
                            world->setBlockAt(x, y, z, BlockType::STONE, true);
                        }
                    }
                }
            }
        }
    }
}
