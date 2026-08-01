#include "MeshBuilder.h"
#include <vector>
#include <cstring>

// Helper to add a face to our vertex data array
static void addFace(std::vector<float>& vertices, int x, int y, int z, const glm::vec4& uvBounds, int face) {
    // Face IDs:
    // 0: Front (+Z)
    // 1: Back (-Z)
    // 2: Left (-X)
    // 3: Right (+X)
    // 4: Bottom (-Y)
    // 5: Top (+Y)
    
    // Each face has 6 vertices (2 triangles), each with 3 pos floats, 3 normal floats, 2 uv floats (8 total)
    
    float fx = static_cast<float>(x);
    float fy = static_cast<float>(y);
    float fz = static_cast<float>(z);
    
    float u0 = uvBounds[0];
    float v0 = uvBounds[1];
    float u1 = uvBounds[2];
    float v1 = uvBounds[3];

    float nx = 0.0f, ny = 0.0f, nz = 0.0f;
    if (face == 0) nz = 1.0f;
    else if (face == 1) nz = -1.0f;
    else if (face == 2) nx = -1.0f;
    else if (face == 3) nx = 1.0f;
    else if (face == 4) ny = -1.0f;
    else if (face == 5) ny = 1.0f;

    float faceVertices[48];

    switch(face) {
        case 0: { // Front
            float front[] = {
                fx,     fy,     fz+1.0f, nx, ny, nz, u0, v0,
                fx+1.0f,fy,     fz+1.0f, nx, ny, nz, u1, v0,
                fx+1.0f,fy+1.0f,fz+1.0f, nx, ny, nz, u1, v1,
                fx,     fy,     fz+1.0f, nx, ny, nz, u0, v0,
                fx+1.0f,fy+1.0f,fz+1.0f, nx, ny, nz, u1, v1,
                fx,     fy+1.0f,fz+1.0f, nx, ny, nz, u0, v1
            };
            std::memcpy(faceVertices, front, sizeof(front));
            break;
        }
        case 1: { // Back
            float back_ccw[] = {
                fx,     fy,     fz, nx, ny, nz, u1, v0,
                fx+1.0f,fy+1.0f,fz, nx, ny, nz, u0, v1,
                fx+1.0f,fy,     fz, nx, ny, nz, u0, v0,
                fx,     fy,     fz, nx, ny, nz, u1, v0,
                fx,     fy+1.0f,fz, nx, ny, nz, u1, v1,
                fx+1.0f,fy+1.0f,fz, nx, ny, nz, u0, v1
            };
            std::memcpy(faceVertices, back_ccw, sizeof(back_ccw));
            break;
        }
        case 2: { // Left
            float left[] = {
                fx, fy,     fz+1.0f, nx, ny, nz, u1, v0,
                fx, fy+1.0f,fz+1.0f, nx, ny, nz, u1, v1,
                fx, fy+1.0f,fz,      nx, ny, nz, u0, v1,
                fx, fy,     fz+1.0f, nx, ny, nz, u1, v0,
                fx, fy+1.0f,fz,      nx, ny, nz, u0, v1,
                fx, fy,     fz,      nx, ny, nz, u0, v0
            };
            std::memcpy(faceVertices, left, sizeof(left));
            break;
        }
        case 3: { // Right
            float right_ccw[] = {
                fx+1.0f, fy,     fz+1.0f, nx, ny, nz, u0, v0,
                fx+1.0f, fy+1.0f,fz,      nx, ny, nz, u1, v1,
                fx+1.0f, fy+1.0f,fz+1.0f, nx, ny, nz, u0, v1,
                fx+1.0f, fy,     fz+1.0f, nx, ny, nz, u0, v0,
                fx+1.0f, fy,     fz,      nx, ny, nz, u1, v0,
                fx+1.0f, fy+1.0f,fz,      nx, ny, nz, u1, v1
            };
            std::memcpy(faceVertices, right_ccw, sizeof(right_ccw));
            break;
        }
        case 4: { // Bottom
            float bottom[] = {
                fx,     fy, fz,      nx, ny, nz, u0, v0,
                fx+1.0f,fy, fz,      nx, ny, nz, u1, v0,
                fx+1.0f,fy, fz+1.0f, nx, ny, nz, u1, v1,
                fx,     fy, fz,      nx, ny, nz, u0, v0,
                fx+1.0f,fy, fz+1.0f, nx, ny, nz, u1, v1,
                fx,     fy, fz+1.0f, nx, ny, nz, u0, v1
            };
            std::memcpy(faceVertices, bottom, sizeof(bottom));
            break;
        }
        case 5: { // Top
            float top_ccw[] = {
                fx,     fy+1.0f, fz,      nx, ny, nz, u0, v1,
                fx+1.0f,fy+1.0f, fz+1.0f, nx, ny, nz, u1, v0,
                fx+1.0f,fy+1.0f, fz,      nx, ny, nz, u1, v1,
                fx,     fy+1.0f, fz,      nx, ny, nz, u0, v1,
                fx,     fy+1.0f, fz+1.0f, nx, ny, nz, u0, v0,
                fx+1.0f,fy+1.0f, fz+1.0f, nx, ny, nz, u1, v0
            };
            std::memcpy(faceVertices, top_ccw, sizeof(top_ccw));
            break;
        }
    }

    vertices.insert(vertices.end(), faceVertices, faceVertices + 48);
}

void MeshBuilder::buildMesh(Chunk* chunk, const World* world) {
    if (!chunk || !world) return;

    std::vector<float> vertices;
    
    // World coordinates for the chunk's base
    int worldBaseX = chunk->getX() * Config::CHUNK_SIZE;
    int worldBaseY = chunk->getY() * Config::CHUNK_SIZE;
    int worldBaseZ = chunk->getZ() * Config::CHUNK_SIZE;

    for (int x = 0; x < Config::CHUNK_SIZE; ++x) {
        for (int y = 0; y < Config::CHUNK_SIZE; ++y) {
            for (int z = 0; z < Config::CHUNK_SIZE; ++z) {
                Block block = chunk->getBlock(x, y, z);
                
                if (!block.active || !isSolidType(block.type)) {
                    continue; // Skip air and inactive blocks
                }

                int wx = worldBaseX + x;
                int wy = worldBaseY + y;
                int wz = worldBaseZ + z;

                // Face Culling & Building
                
                // Front (+Z)
                if (!world->isSolidAt(wx, wy, wz + 1)) {
                    addFace(vertices, wx, wy, wz, getBlockUVBounds(block.type, 0), 0);
                }
                // Back (-Z)
                if (!world->isSolidAt(wx, wy, wz - 1)) {
                    addFace(vertices, wx, wy, wz, getBlockUVBounds(block.type, 1), 1);
                }
                // Left (-X)
                if (!world->isSolidAt(wx - 1, wy, wz)) {
                    addFace(vertices, wx, wy, wz, getBlockUVBounds(block.type, 2), 2);
                }
                // Right (+X)
                if (!world->isSolidAt(wx + 1, wy, wz)) {
                    addFace(vertices, wx, wy, wz, getBlockUVBounds(block.type, 3), 3);
                }
                // Bottom (-Y)
                if (!world->isSolidAt(wx, wy - 1, wz)) {
                    addFace(vertices, wx, wy, wz, getBlockUVBounds(block.type, 4), 4);
                }
                // Top (+Y)
                if (!world->isSolidAt(wx, wy + 1, wz)) {
                    addFace(vertices, wx, wy, wz, getBlockUVBounds(block.type, 5), 5);
                }
            }
        }
    }

    // Now upload to OpenGL
    if (chunk->VAO == 0) {
        glGenVertexArrays(1, &chunk->VAO);
        glGenBuffers(1, &chunk->VBO);
    }

    glBindVertexArray(chunk->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, chunk->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // 8 floats per vertex: [x, y, z, nx, ny, nz, u, v]
    
    // Position (x,y,z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Normal (nx,ny,nz)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // UV (u,v)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);

    // 8 floats per vertex, 6 vertices per face
    chunk->vertexCount = vertices.size() / 8;
    chunk->setDirty(false);
}
