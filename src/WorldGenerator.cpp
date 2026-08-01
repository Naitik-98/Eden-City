#include "WorldGenerator.h"

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
