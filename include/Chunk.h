#pragma once

#include "Block.h"
#include "Config.h"
#include <glad/glad.h>

class Chunk {
public:
    Chunk(int chunkX, int chunkY, int chunkZ);
    ~Chunk();

    // Prevent copying because Chunk manages OpenGL resources (VAO/VBO)
    Chunk(const Chunk&) = delete;
    Chunk& operator=(const Chunk&) = delete;

    // Get block at local coordinates [0, CHUNK_SIZE-1]
    Block getBlock(int x, int y, int z) const;
    
    // Set block at local coordinates
    void setBlock(int x, int y, int z, BlockType type, bool active = true);

    // Get chunk's world-grid coordinates
    int getX() const { return m_chunkX; }
    int getY() const { return m_chunkY; }
    int getZ() const { return m_chunkZ; }

    bool isDirty() const { return m_isDirty; }
    void setDirty(bool dirty) { m_isDirty = dirty; }

    // OpenGL Mesh identifiers (managed by MeshBuilder/Renderer)
    GLuint VAO = 0;
    GLuint VBO = 0;
    int vertexCount = 0;

private:
    int m_chunkX, m_chunkY, m_chunkZ;
    bool m_isDirty = true;
    
    // 3D array of blocks: [x][y][z]
    Block m_blocks[Config::CHUNK_SIZE][Config::CHUNK_SIZE][Config::CHUNK_SIZE];
};
