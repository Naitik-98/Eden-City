#include "WorldGenerator.h"
#include "CrystalTowerEntity.h"

#define STB_PERLIN_IMPLEMENTATION
#include "../lib/stb/stb_perlin.h"
#include <cstdlib>

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
                Block b = world->getBlockAt(x, y, z);
                if (b.type == BlockType::DIRT && b.active) {
                    // Check block above
                    Block above = world->getBlockAt(x, y + 1, z);
                    if (!above.active || above.type == BlockType::AIR) {
                        // Exposed to air -> Grass
                        world->setBlockAt(x, y, z, BlockType::GRASS, true);
                    } else {
                        // Check block deeply below to convert to stone
                        Block below1 = world->getBlockAt(x, y - 1, z);
                        Block below2 = world->getBlockAt(x, y - 2, z);
                        if (below1.active && below2.active) {
                            world->setBlockAt(x, y, z, BlockType::STONE, true);
                        }
                    }
                }
            }
        }
    }
    std::vector<glm::vec3> nodes = generateBuildings(world);
    generateRoads(world, nodes);
    generateCrystalTowers(world);
}

std::vector<glm::vec3> WorldGenerator::generateBuildings(World* world) {
    std::vector<glm::vec3> nodes;
    if (!world) return nodes;

    int maxWorldX = Config::WORLD_CHUNKS_X * Config::CHUNK_SIZE;
    int maxWorldY = Config::WORLD_CHUNKS_Y * Config::CHUNK_SIZE;
    int maxWorldZ = Config::WORLD_CHUNKS_Z * Config::CHUNK_SIZE;

    // Grid-based random placement
    for (int x = 8; x < maxWorldX - 8; x += 16) {
        for (int z = 8; z < maxWorldZ - 8; z += 16) {
            // Find highest surface block
            int surfaceY = -1;
            for (int y = maxWorldY - 1; y >= 0; --y) {
                Block b = world->getBlockAt(x, y, z);
                if (b.active && b.type == BlockType::GRASS) {
                    surfaceY = y;
                    break;
                }
            }

            if (surfaceY != -1) {
                // Random chance to spawn a building in this 16x16 cell
                if ((std::rand() % 100) < 35) { // 35% chance
                    int bWidth = 4 + (std::rand() % 5);  // 4 to 8
                    int bDepth = 4 + (std::rand() % 5);  // 4 to 8
                    int bHeight = 15 + (std::rand() % 25); // 15 to 39

                    int startX = x - bWidth / 2;
                    int startZ = z - bDepth / 2;

                    for (int bx = 0; bx < bWidth; ++bx) {
                        for (int bz = 0; bz < bDepth; ++bz) {
                            for (int by = 0; by < bHeight; ++by) {
                                BlockType type = BlockType::METAL;
                                
                                // Add crystal windows on the outside
                                bool isOutside = (bx == 0 || bx == bWidth - 1 || bz == 0 || bz == bDepth - 1);
                                if (isOutside && by > 2 && by < bHeight - 2) {
                                    if ((by % 4 == 0) && ((std::rand() % 100) < 40)) {
                                        type = BlockType::CRYSTAL;
                                    }
                                }
                                
                                world->setBlockAt(startX + bx, surfaceY + 1 + by, startZ + bz, type, true);
                            }
                        }
                    }
                    
                    // Add building center to node list for road connections
                    nodes.push_back(glm::vec3(x, surfaceY + 1, z));
                }
            }
        }
    }
    
    return nodes;
}

void WorldGenerator::generateRoads(World* world, const std::vector<glm::vec3>& nodes) {
    if (!world || nodes.size() < 2) return;

    // Connect each building to its nearest neighbor
    for (size_t i = 0; i < nodes.size(); ++i) {
        glm::vec3 start = nodes[i];
        
        float minDist = 999999.0f;
        int closestIdx = -1;

        for (size_t j = 0; j < nodes.size(); ++j) {
            if (i == j) continue;
            float dist = glm::distance(start, nodes[j]);
            if (dist < minDist) {
                minDist = dist;
                closestIdx = j;
            }
        }

        if (closestIdx != -1) {
            glm::vec3 end = nodes[closestIdx];
            int steps = static_cast<int>(std::ceil(minDist));
            
            for (int step = 0; step <= steps; ++step) {
                float t = (steps == 0) ? 0.0f : (static_cast<float>(step) / steps);
                glm::vec3 pos = start + (end - start) * t;
                
                int bx = static_cast<int>(std::round(pos.x));
                int by = static_cast<int>(std::round(pos.y));
                int bz = static_cast<int>(std::round(pos.z));

                // 3-wide road cross shape
                world->setBlockAt(bx, by, bz, BlockType::STONE, true);
                world->setBlockAt(bx + 1, by, bz, BlockType::STONE, true);
                world->setBlockAt(bx - 1, by, bz, BlockType::STONE, true);
                world->setBlockAt(bx, by, bz + 1, BlockType::STONE, true);
                world->setBlockAt(bx, by, bz - 1, BlockType::STONE, true);
            }
        }
    }
}

void WorldGenerator::generateCrystalTowers(World* world) {
    if (!world) return;

    int maxWorldX = Config::WORLD_CHUNKS_X * Config::CHUNK_SIZE;
    int maxWorldY = Config::WORLD_CHUNKS_Y * Config::CHUNK_SIZE;
    int maxWorldZ = Config::WORLD_CHUNKS_Z * Config::CHUNK_SIZE;

    // Grid-based random placement with wide spacing
    for (int x = 16; x < maxWorldX - 16; x += 32) {
        for (int z = 16; z < maxWorldZ - 16; z += 32) {
            // Find highest surface block
            int surfaceY = -1;
            for (int y = maxWorldY - 1; y >= 0; --y) {
                Block b = world->getBlockAt(x, y, z);
                if (b.active && b.type == BlockType::GRASS) {
                    surfaceY = y;
                    break;
                }
            }

            if (surfaceY != -1) {
                // Rare chance to spawn a tower in this 32x32 cell
                if ((std::rand() % 100) < 15) { // 15% chance per 32x32 area
                    int tHeight = 40 + (std::rand() % 20); // 40 to 59 blocks high

                    for (int by = 0; by < tHeight; ++by) {
                        int radius = 0;
                        if (by < 5) radius = 2; // 5x5
                        else if (by < tHeight - 10) radius = 1; // 3x3
                        else radius = 0; // 1x1

                        BlockType type = BlockType::CRYSTAL;
                        if (by < 2) type = BlockType::STONE; // Foundation

                        for (int bx = -radius; bx <= radius; ++bx) {
                            for (int bz = -radius; bz <= radius; ++bz) {
                                world->setBlockAt(x + bx, surfaceY + 1 + by, z + bz, type, true);
                            }
                        }
                    }
                    
                    // Spawn a CrystalTowerEntity at the base of this tower
                    glm::vec3 towerPos = glm::vec3(x, surfaceY + 1, z);
                    world->addEntity(std::make_unique<CrystalTowerEntity>(towerPos));
                }
            }
        }
    }
}
