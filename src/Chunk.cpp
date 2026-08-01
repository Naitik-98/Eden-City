#include "Chunk.h"
#include <cstring> // For memset (optional, but good for initialization if needed, though constructor initializes to AIR)

Chunk::Chunk(int chunkX, int chunkY, int chunkZ) 
    : m_chunkX(chunkX), m_chunkY(chunkY), m_chunkZ(chunkZ) {
    // Blocks are implicitly initialized to AIR by the Block struct's default constructor
}

Chunk::~Chunk() {
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
    if (VBO != 0) glDeleteBuffers(1, &VBO);
}

Block Chunk::getBlock(int x, int y, int z) const {
    if (x < 0 || x >= Config::CHUNK_SIZE ||
        y < 0 || y >= Config::CHUNK_SIZE ||
        z < 0 || z >= Config::CHUNK_SIZE) {
        return Block{BlockType::AIR, false};
    }
    return m_blocks[x][y][z];
}

void Chunk::setBlock(int x, int y, int z, BlockType type, bool active) {
    if (x >= 0 && x < Config::CHUNK_SIZE &&
        y >= 0 && y < Config::CHUNK_SIZE &&
        z >= 0 && z < Config::CHUNK_SIZE) {
        
        // Only mark dirty if the block is actually changing state
        if (m_blocks[x][y][z].type != type || m_blocks[x][y][z].active != active) {
            m_blocks[x][y][z].type = type;
            m_blocks[x][y][z].active = active;
            m_isDirty = true;
        }
    }
}
