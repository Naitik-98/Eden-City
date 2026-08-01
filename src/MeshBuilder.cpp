#include "MeshBuilder.h"
#include <vector>
#include <cstring>

// Helper to add a face to our vertex data array
static void addFace(std::vector<float>& vertices, int x, int y, int z, const glm::vec3& color, int face) {
    // Face IDs:
    // 0: Front (+Z)
    // 1: Back (-Z)
    // 2: Left (-X)
    // 3: Right (+X)
    // 4: Bottom (-Y)
    // 5: Top (+Y)
    
    // Each face has 6 vertices (2 triangles), each with 3 pos floats and 3 color floats
    
    float fx = static_cast<float>(x);
    float fy = static_cast<float>(y);
    float fz = static_cast<float>(z);
    
    float r = color.r;
    float g = color.g;
    float b = color.b;

    // Apply some fake ambient occlusion / directional lighting based on face direction
    if (face == 0 || face == 1) { r *= 0.8f; g *= 0.8f; b *= 0.8f; }
    if (face == 2 || face == 3) { r *= 0.6f; g *= 0.6f; b *= 0.6f; }
    if (face == 4) { r *= 0.4f; g *= 0.4f; b *= 0.4f; } // Bottom is darkest
    // Top (5) is full brightness

    float faceVertices[36];

    switch(face) {
        case 0: { // Front
            float front[] = {
                fx,     fy,     fz+1.0f, r, g, b,
                fx+1.0f,fy,     fz+1.0f, r, g, b,
                fx+1.0f,fy+1.0f,fz+1.0f, r, g, b,
                fx,     fy,     fz+1.0f, r, g, b,
                fx+1.0f,fy+1.0f,fz+1.0f, r, g, b,
                fx,     fy+1.0f,fz+1.0f, r, g, b
            };
            std::memcpy(faceVertices, front, sizeof(front));
            break;
        }
        case 1: { // Back
            // Need to reverse winding order for back face so it faces outward
            float back_ccw[] = {
                fx,     fy,     fz, r, g, b,
                fx+1.0f,fy+1.0f,fz, r, g, b,
                fx+1.0f,fy,     fz, r, g, b,
                fx,     fy,     fz, r, g, b,
                fx,     fy+1.0f,fz, r, g, b,
                fx+1.0f,fy+1.0f,fz, r, g, b
            };
            std::memcpy(faceVertices, back_ccw, sizeof(back_ccw));
            break;
        }
        case 2: { // Left
            float left[] = {
                fx, fy,     fz+1.0f, r, g, b,
                fx, fy+1.0f,fz+1.0f, r, g, b,
                fx, fy+1.0f,fz,      r, g, b,
                fx, fy,     fz+1.0f, r, g, b,
                fx, fy+1.0f,fz,      r, g, b,
                fx, fy,     fz,      r, g, b
            };
            std::memcpy(faceVertices, left, sizeof(left));
            break;
        }
        case 3: { // Right
            float right_ccw[] = {
                fx+1.0f, fy,     fz+1.0f, r, g, b,
                fx+1.0f, fy+1.0f,fz,      r, g, b,
                fx+1.0f, fy+1.0f,fz+1.0f, r, g, b,
                fx+1.0f, fy,     fz+1.0f, r, g, b,
                fx+1.0f, fy,     fz,      r, g, b,
                fx+1.0f, fy+1.0f,fz,      r, g, b
            };
            std::memcpy(faceVertices, right_ccw, sizeof(right_ccw));
            break;
        }
        case 4: { // Bottom
            float bottom[] = {
                fx,     fy, fz,      r, g, b,
                fx+1.0f,fy, fz,      r, g, b,
                fx+1.0f,fy, fz+1.0f, r, g, b,
                fx,     fy, fz,      r, g, b,
                fx+1.0f,fy, fz+1.0f, r, g, b,
                fx,     fy, fz+1.0f, r, g, b
            };
            std::memcpy(faceVertices, bottom, sizeof(bottom));
            break;
        }
        case 5: { // Top
            float top_ccw[] = {
                fx,     fy+1.0f, fz,      r, g, b,
                fx+1.0f,fy+1.0f, fz+1.0f, r, g, b,
                fx+1.0f,fy+1.0f, fz,      r, g, b,
                fx,     fy+1.0f, fz,      r, g, b,
                fx,     fy+1.0f, fz+1.0f, r, g, b,
                fx+1.0f,fy+1.0f, fz+1.0f, r, g, b
            };
            std::memcpy(faceVertices, top_ccw, sizeof(top_ccw));
            break;
        }
    }

    vertices.insert(vertices.end(), faceVertices, faceVertices + 36);
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

                glm::vec3 color = getBlockColor(block.type);
                int wx = worldBaseX + x;
                int wy = worldBaseY + y;
                int wz = worldBaseZ + z;

                // Face Culling: Check adjacent blocks. If adjacent is solid, skip that face.
                
                // Front (+Z)
                if (!world->isSolidAt(wx, wy, wz + 1)) {
                    addFace(vertices, wx, wy, wz, color, 0);
                }
                // Back (-Z)
                if (!world->isSolidAt(wx, wy, wz - 1)) {
                    addFace(vertices, wx, wy, wz, color, 1);
                }
                // Left (-X)
                if (!world->isSolidAt(wx - 1, wy, wz)) {
                    addFace(vertices, wx, wy, wz, color, 2);
                }
                // Right (+X)
                if (!world->isSolidAt(wx + 1, wy, wz)) {
                    addFace(vertices, wx, wy, wz, color, 3);
                }
                // Bottom (-Y)
                if (!world->isSolidAt(wx, wy - 1, wz)) {
                    addFace(vertices, wx, wy, wz, color, 4);
                }
                // Top (+Y)
                if (!world->isSolidAt(wx, wy + 1, wz)) {
                    addFace(vertices, wx, wy, wz, color, 5);
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

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);

    // 6 floats per vertex, 6 vertices per face -> 1 face = 36 floats
    // vertex count = floats / 6
    chunk->vertexCount = vertices.size() / 6;
    chunk->setDirty(false);
}
