#pragma once

#include "World.h"
#include <vector>
#include <glm/glm.hpp>

class WorldGenerator {
public:
    // Populates the world with flat terrain: stone base, dirt middle, grass top
    static void generateFlatWorld(World* world);

    // Generates floating islands using 3D noise
    static void generateEdenCity(World* world);

    // Generates procedural structures on top of the islands
    static std::vector<glm::vec3> generateBuildings(World* world);

    // Generates bridges to connect the given building nodes
    static void generateRoads(World* world, const std::vector<glm::vec3>& nodes);

    // Generates rare Crystal Towers across the map
    static void generateCrystalTowers(World* world);
};
