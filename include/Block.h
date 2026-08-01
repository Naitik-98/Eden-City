// =============================================================================
// Block.h — Voxel Block Definition
// =============================================================================
// A Block is the smallest unit of the voxel world.
//
// Design decisions:
//   - BlockType is a uint8_t enum: only 256 types max, but each block is tiny
//     in memory (2 bytes). A 16^3 chunk = 8,192 bytes. Very cache-friendly.
//   - 'active' flag: allows toggling visibility without changing type.
//     Useful for: undo, ghost-block preview, animation, soft-delete.
//   - getBlockUVBounds(): Week 2 replaces solid colors with texture atlasing.
//     Keeping this as a free function (not a method) because Block is a value
//     type — it should stay as lightweight as possible.
// =============================================================================

#pragma once

#include <cstdint>
#include <glm/glm.hpp>

// -----------------------------------------------------------------------------
// BlockType — every distinct material in the world
// -----------------------------------------------------------------------------
enum class BlockType : uint8_t {
    AIR = 0,        // empty space — not rendered, not solid
    GRASS,          // surface layer
    STONE,          // underground / structures
    DIRT,           // below grass
    CRYSTAL,        // Eden City special — futuristic element
    METAL,          // Eden City special — structural
    COUNT           // always last — gives total number of types
};

// -----------------------------------------------------------------------------
// Block — a single voxel
// -----------------------------------------------------------------------------
struct Block {
    BlockType type   = BlockType::AIR;
    bool      active = false;   // only active blocks are rendered and collidable
};

// -----------------------------------------------------------------------------
// Helper: is this block type solid? (used for collision + face culling)
// AIR is never solid. Inactive blocks are handled separately.
// -----------------------------------------------------------------------------
inline bool isSolidType(BlockType type) {
    return type != BlockType::AIR;
}

// -----------------------------------------------------------------------------
// Helper: get the UV bounds for a block type's texture in the atlas.
// Returns {u0, v0, u1, v1}
// Atlas is 2x2: 
// (0,1): Grass  (1,1): Dirt
// (0,0): Stone  (1,0): Metal
// -----------------------------------------------------------------------------
inline glm::vec4 getBlockUVBounds(BlockType type, int face) {
    // face 5 is top, face 4 is bottom, others are sides
    float u0 = 0.0f, v0 = 0.0f, u1 = 0.5f, v1 = 0.5f;

    switch (type) {
        case BlockType::GRASS:
            if (face == 5) { u0 = 0.0f; v0 = 0.5f; } // Top: Grass top
            else if (face == 4) { u0 = 0.5f; v0 = 0.5f; } // Bottom: Dirt
            else { u0 = 0.5f; v0 = 0.5f; } // Side: Dirt (Normally side grass, using dirt for now)
            break;
        case BlockType::DIRT:
            u0 = 0.5f; v0 = 0.5f; // Top-Right
            break;
        case BlockType::STONE:
            u0 = 0.0f; v0 = 0.0f; // Bottom-Left
            break;
        case BlockType::CRYSTAL:
        case BlockType::METAL:
            u0 = 0.5f; v0 = 0.0f; // Bottom-Right
            break;
        default:
            u0 = 0.0f; v0 = 0.0f;
            break;
    }
    u1 = u0 + 0.5f;
    v1 = v0 + 0.5f;

    return glm::vec4(u0, v0, u1, v1);
}
