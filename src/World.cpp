#include "World.h"
#include "InputManager.h"
#include <cmath>
#include <iostream>

World::World() {
}

void World::init() {
    m_chunks.clear();
    // Pre-allocate chunks
    for (int cx = 0; cx < Config::WORLD_CHUNKS_X; ++cx) {
        for (int cy = 0; cy < Config::WORLD_CHUNKS_Y; ++cy) {
            for (int cz = 0; cz < Config::WORLD_CHUNKS_Z; ++cz) {
                m_chunks.push_back(std::make_unique<Chunk>(cx, cy, cz));
            }
        }
    }
}

Chunk* World::getChunk(int cx, int cy, int cz) const {
    if (cx < 0 || cx >= Config::WORLD_CHUNKS_X ||
        cy < 0 || cy >= Config::WORLD_CHUNKS_Y ||
        cz < 0 || cz >= Config::WORLD_CHUNKS_Z) {
        return nullptr;
    }
    
    // Calculate 1D index from 3D coordinates (assuming X, then Y, then Z loop order as in init)
    // index = cx * (Y * Z) + cy * Z + cz
    int index = cx * (Config::WORLD_CHUNKS_Y * Config::WORLD_CHUNKS_Z) 
              + cy * Config::WORLD_CHUNKS_Z 
              + cz;
              
    return m_chunks[index].get();
}

Block World::getBlockAt(int worldX, int worldY, int worldZ) const {
    // Determine which chunk this block belongs to
    int cx = worldX / Config::CHUNK_SIZE;
    int cy = worldY / Config::CHUNK_SIZE;
    int cz = worldZ / Config::CHUNK_SIZE;

    // Handle negative coordinates correctly (integer division truncates towards zero)
    if (worldX < 0) cx = (worldX + 1) / Config::CHUNK_SIZE - 1;
    if (worldY < 0) cy = (worldY + 1) / Config::CHUNK_SIZE - 1;
    if (worldZ < 0) cz = (worldZ + 1) / Config::CHUNK_SIZE - 1;

    Chunk* chunk = getChunk(cx, cy, cz);
    if (!chunk) {
        return Block{BlockType::AIR, false};
    }

    // Determine local coordinates within the chunk
    int lx = worldX - cx * Config::CHUNK_SIZE;
    int ly = worldY - cy * Config::CHUNK_SIZE;
    int lz = worldZ - cz * Config::CHUNK_SIZE;

    return chunk->getBlock(lx, ly, lz);
}

Block World::getBlockAt(float worldX, float worldY, float worldZ) const {
    return getBlockAt(static_cast<int>(std::floor(worldX)),
                      static_cast<int>(std::floor(worldY)),
                      static_cast<int>(std::floor(worldZ)));
}

void World::setBlockAt(int worldX, int worldY, int worldZ, BlockType type, bool active) {
    int cx = worldX / Config::CHUNK_SIZE;
    int cy = worldY / Config::CHUNK_SIZE;
    int cz = worldZ / Config::CHUNK_SIZE;

    if (worldX < 0) cx = (worldX + 1) / Config::CHUNK_SIZE - 1;
    if (worldY < 0) cy = (worldY + 1) / Config::CHUNK_SIZE - 1;
    if (worldZ < 0) cz = (worldZ + 1) / Config::CHUNK_SIZE - 1;

    Chunk* chunk = getChunk(cx, cy, cz);
    if (chunk) {
        int lx = worldX - cx * Config::CHUNK_SIZE;
        int ly = worldY - cy * Config::CHUNK_SIZE;
        int lz = worldZ - cz * Config::CHUNK_SIZE;
        chunk->setBlock(lx, ly, lz, type, active);
        
        // If the block is on a chunk boundary, we should potentially mark adjacent chunks as dirty
        // because their face culling might need to be updated.
        if (lx == 0) {
            Chunk* neighbor = getChunk(cx - 1, cy, cz);
            if (neighbor) neighbor->setDirty(true);
        } else if (lx == Config::CHUNK_SIZE - 1) {
            Chunk* neighbor = getChunk(cx + 1, cy, cz);
            if (neighbor) neighbor->setDirty(true);
        }
        
        if (ly == 0) {
            Chunk* neighbor = getChunk(cx, cy - 1, cz);
            if (neighbor) neighbor->setDirty(true);
        } else if (ly == Config::CHUNK_SIZE - 1) {
            Chunk* neighbor = getChunk(cx, cy + 1, cz);
            if (neighbor) neighbor->setDirty(true);
        }
        
        if (lz == 0) {
            Chunk* neighbor = getChunk(cx, cy, cz - 1);
            if (neighbor) neighbor->setDirty(true);
        } else if (lz == Config::CHUNK_SIZE - 1) {
            Chunk* neighbor = getChunk(cx, cy, cz + 1);
            if (neighbor) neighbor->setDirty(true);
        }
    }
}

void World::setBlockAt(float worldX, float worldY, float worldZ, BlockType type, bool active) {
    setBlockAt(static_cast<int>(std::floor(worldX)),
               static_cast<int>(std::floor(worldY)),
               static_cast<int>(std::floor(worldZ)), type, active);
}

bool World::isSolidAt(int worldX, int worldY, int worldZ) const {
    Block block = getBlockAt(worldX, worldY, worldZ);
    return block.active && isSolidType(block.type);
}

bool World::isSolidAt(float worldX, float worldY, float worldZ) const {
    return isSolidAt(static_cast<int>(std::floor(worldX)),
                     static_cast<int>(std::floor(worldY)),
                     static_cast<int>(std::floor(worldZ)));
}

void World::updateEntities(float dt, InputManager* input) {
    for (const auto& entity : m_entities) {
        entity->update(dt, this, input);
    }
}
