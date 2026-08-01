#include "Raycaster.h"
#include <cmath>

RaycastResult Raycaster::raycast(const World* world, glm::vec3 origin, glm::vec3 direction, float maxDistance) {
    RaycastResult result;
    if (!world) return result;

    // Normalize direction just in case
    direction = glm::normalize(direction);

    // Initial voxel coordinates
    glm::ivec3 voxel(
        std::floor(origin.x),
        std::floor(origin.y),
        std::floor(origin.z)
    );

    // Step direction (-1 or 1) for each axis
    glm::ivec3 step(
        (direction.x > 0.0f) ? 1 : -1,
        (direction.y > 0.0f) ? 1 : -1,
        (direction.z > 0.0f) ? 1 : -1
    );

    // Distance to cross next voxel boundary
    glm::vec3 tMax;
    glm::vec3 tDelta;

    // Calculate initial tMax and tDelta
    for (int i = 0; i < 3; ++i) {
        if (direction[i] != 0.0f) {
            tDelta[i] = std::abs(1.0f / direction[i]);
            if (step[i] > 0) {
                tMax[i] = (voxel[i] + 1.0f - origin[i]) * tDelta[i];
            } else {
                tMax[i] = (origin[i] - voxel[i]) * tDelta[i];
            }
        } else {
            tDelta[i] = 1000000.0f; // effectively infinity
            tMax[i] = 1000000.0f;
        }
    }

    float t = 0.0f;
    glm::ivec3 normal(0);

    // 3D DDA loop
    while (t <= maxDistance) {
        if (world->isSolidAt(voxel.x, voxel.y, voxel.z)) {
            result.hit = true;
            result.hitBlockPos = voxel;
            result.hitNormal = normal;
            return result;
        }

        // Advance to the next voxel
        if (tMax.x < tMax.y) {
            if (tMax.x < tMax.z) {
                voxel.x += step.x;
                t = tMax.x;
                tMax.x += tDelta.x;
                normal = glm::ivec3(-step.x, 0, 0);
            } else {
                voxel.z += step.z;
                t = tMax.z;
                tMax.z += tDelta.z;
                normal = glm::ivec3(0, 0, -step.z);
            }
        } else {
            if (tMax.y < tMax.z) {
                voxel.y += step.y;
                t = tMax.y;
                tMax.y += tDelta.y;
                normal = glm::ivec3(0, -step.y, 0);
            } else {
                voxel.z += step.z;
                t = tMax.z;
                tMax.z += tDelta.z;
                normal = glm::ivec3(0, 0, -step.z);
            }
        }
    }

    return result;
}
