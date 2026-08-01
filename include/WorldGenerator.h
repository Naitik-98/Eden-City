#pragma once

#include "World.h"

class WorldGenerator {
public:
    // Populates the world with flat terrain: stone base, dirt middle, grass top
    static void generateFlatWorld(World* world);

    // Generates floating islands using 3D noise
    static void generateEdenCity(World* world);
};
