#pragma once

#include "World.h"
#include <glm/glm.hpp>

// Result of a raycast
struct RaycastResult {
    bool hit = false;
    glm::ivec3 hitBlockPos = glm::ivec3(0); // The block that was hit
    glm::ivec3 hitNormal = glm::ivec3(0);   // The normal of the face that was hit (e.g., 0, 1, 0 for top face)
};

class Raycaster {
public:
    // Performs a 3D DDA (Digital Differential Analyzer) voxel raycast
    // Starts at `origin` and moves along `direction` up to `maxDistance`
    // Returns the first solid block hit.
    static RaycastResult raycast(const World* world, glm::vec3 origin, glm::vec3 direction, float maxDistance);
};
