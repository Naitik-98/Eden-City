#pragma once

#include "Chunk.h"
#include "World.h"

class MeshBuilder {
public:
    // Rebuilds the chunk's VAO/VBO.
    // Face culling is applied: faces touching solid blocks are not rendered.
    // Requires a pointer to the World to check adjacent chunks at boundaries.
    static void buildMesh(Chunk* chunk, const World* world);
};
