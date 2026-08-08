#pragma once

#include <string>
#include <cstdint>

class World;

class WorldSerializer {
public:
    // Saves the entire world's block data to a binary file.
    // Returns true on success, false on failure.
    static bool save(const World* world, const std::string& filePath);

    // Loads block data from a binary file into the world.
    // Marks all affected chunks as dirty so meshes rebuild.
    // Returns true if file was found and loaded, false otherwise.
    static bool load(World* world, const std::string& filePath);

private:
    // Magic number to identify valid Eden City save files: "ECTY"
    static constexpr uint32_t MAGIC = 0x59544345;
    // Increment this when save format changes
    static constexpr uint8_t VERSION = 1;
};
