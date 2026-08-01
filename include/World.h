#pragma once

#include "Chunk.h"
#include <vector>
#include <memory>

class World {
public:
    World();
    ~World() = default;

    // Initializes the world chunks array based on Config::WORLD_CHUNKS_*
    void init();

    // World coordinate conversions
    // Returns the Block at the given absolute world position
    Block getBlockAt(float worldX, float worldY, float worldZ) const;
    Block getBlockAt(int worldX, int worldY, int worldZ) const;

    // Sets a block at the given absolute world position
    void setBlockAt(float worldX, float worldY, float worldZ, BlockType type, bool active = true);
    void setBlockAt(int worldX, int worldY, int worldZ, BlockType type, bool active = true);

    // Checks if the block at the given world position is solid
    bool isSolidAt(float worldX, float worldY, float worldZ) const;
    bool isSolidAt(int worldX, int worldY, int worldZ) const;

    // Retrieve a specific chunk by its grid coordinate (not world coordinate)
    Chunk* getChunk(int cx, int cy, int cz) const;

    // Access to all chunks for rendering
    const std::vector<std::unique_ptr<Chunk>>& getChunks() const { return m_chunks; }

private:
    std::vector<std::unique_ptr<Chunk>> m_chunks;
};
