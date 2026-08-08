#include "WorldSerializer.h"
#include "World.h"
#include "Block.h"
#include "Config.h"

#include <fstream>
#include <iostream>
#include <cstdint>
#include <filesystem>

bool WorldSerializer::save(const World* world, const std::string& filePath) {
    if (!world) return false;

    // Create save directory if it doesn't exist
    std::filesystem::path saveDir = std::filesystem::path(filePath).parent_path();
    if (!saveDir.empty()) {
        std::filesystem::create_directories(saveDir);
    }

    std::ofstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "[Save] ERROR: Could not open file for writing: " << filePath << "\n";
        return false;
    }

    // Write header
    file.write(reinterpret_cast<const char*>(&MAGIC), sizeof(MAGIC));
    file.write(reinterpret_cast<const char*>(&VERSION), sizeof(VERSION));

    // Write world dimensions for validation
    int32_t wx = Config::WORLD_CHUNKS_X;
    int32_t wy = Config::WORLD_CHUNKS_Y;
    int32_t wz = Config::WORLD_CHUNKS_Z;
    int32_t cs = Config::CHUNK_SIZE;
    file.write(reinterpret_cast<const char*>(&wx), sizeof(wx));
    file.write(reinterpret_cast<const char*>(&wy), sizeof(wy));
    file.write(reinterpret_cast<const char*>(&wz), sizeof(wz));
    file.write(reinterpret_cast<const char*>(&cs), sizeof(cs));

    // Write each chunk's block data
    for (int cx = 0; cx < Config::WORLD_CHUNKS_X; ++cx) {
        for (int cy = 0; cy < Config::WORLD_CHUNKS_Y; ++cy) {
            for (int cz = 0; cz < Config::WORLD_CHUNKS_Z; ++cz) {
                const Chunk* chunk = world->getChunk(cx, cy, cz);
                if (!chunk) continue;

                // Write each block: type (1 byte) + active (1 byte)
                for (int x = 0; x < Config::CHUNK_SIZE; ++x) {
                    for (int y = 0; y < Config::CHUNK_SIZE; ++y) {
                        for (int z = 0; z < Config::CHUNK_SIZE; ++z) {
                            Block b = chunk->getBlock(x, y, z);
                            uint8_t type = static_cast<uint8_t>(b.type);
                            uint8_t active = b.active ? 1 : 0;
                            file.write(reinterpret_cast<const char*>(&type), 1);
                            file.write(reinterpret_cast<const char*>(&active), 1);
                        }
                    }
                }
            }
        }
    }

    file.close();
    std::cout << "[Save] World saved to: " << filePath << "\n";
    return true;
}

bool WorldSerializer::load(World* world, const std::string& filePath) {
    if (!world) return false;

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        // Not an error — just no save file yet
        std::cout << "[Save] No save file found at: " << filePath << ". Starting fresh.\n";
        return false;
    }

    // Validate header
    uint32_t magic = 0;
    uint8_t version = 0;
    file.read(reinterpret_cast<char*>(&magic), sizeof(magic));
    file.read(reinterpret_cast<char*>(&version), sizeof(version));

    if (magic != MAGIC) {
        std::cerr << "[Save] ERROR: Invalid save file (bad magic). Ignoring.\n";
        return false;
    }
    if (version != VERSION) {
        std::cerr << "[Save] ERROR: Incompatible save version " << (int)version
                  << " (expected " << (int)VERSION << "). Ignoring.\n";
        return false;
    }

    // Validate world dimensions
    int32_t wx, wy, wz, cs;
    file.read(reinterpret_cast<char*>(&wx), sizeof(wx));
    file.read(reinterpret_cast<char*>(&wy), sizeof(wy));
    file.read(reinterpret_cast<char*>(&wz), sizeof(wz));
    file.read(reinterpret_cast<char*>(&cs), sizeof(cs));

    if (wx != Config::WORLD_CHUNKS_X || wy != Config::WORLD_CHUNKS_Y ||
        wz != Config::WORLD_CHUNKS_Z || cs != Config::CHUNK_SIZE) {
        std::cerr << "[Save] ERROR: Save file world dimensions mismatch. Ignoring.\n";
        return false;
    }

    // Read each chunk's block data
    for (int cx = 0; cx < Config::WORLD_CHUNKS_X; ++cx) {
        for (int cy = 0; cy < Config::WORLD_CHUNKS_Y; ++cy) {
            for (int cz = 0; cz < Config::WORLD_CHUNKS_Z; ++cz) {
                Chunk* chunk = world->getChunk(cx, cy, cz);
                if (!chunk) continue;

                for (int x = 0; x < Config::CHUNK_SIZE; ++x) {
                    for (int y = 0; y < Config::CHUNK_SIZE; ++y) {
                        for (int z = 0; z < Config::CHUNK_SIZE; ++z) {
                            uint8_t type = 0, active = 0;
                            file.read(reinterpret_cast<char*>(&type), 1);
                            file.read(reinterpret_cast<char*>(&active), 1);
                            chunk->setBlock(x, y, z, static_cast<BlockType>(type), active != 0);
                        }
                    }
                }

                // Mark dirty so MeshBuilder rebuilds the chunk mesh
                chunk->setDirty(true);
            }
        }
    }

    file.close();
    std::cout << "[Save] World loaded from: " << filePath << "\n";
    return true;
}
