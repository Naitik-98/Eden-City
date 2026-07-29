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
//   - getBlockColor(): Week 1 uses solid colors. Week 2 replaces with textures.
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
// Helper: get the display color for a block type (Week 1 — no textures)
// Returns magenta for unknown types so they're immediately visible as bugs.
// -----------------------------------------------------------------------------
inline glm::vec3 getBlockColor(BlockType type) {
    switch (type) {
        case BlockType::GRASS:   return glm::vec3(0.30f, 0.70f, 0.20f);  // natural green
        case BlockType::STONE:   return glm::vec3(0.50f, 0.50f, 0.50f);  // neutral gray
        case BlockType::DIRT:    return glm::vec3(0.55f, 0.35f, 0.15f);  // earthy brown
        case BlockType::CRYSTAL: return glm::vec3(0.20f, 0.85f, 0.90f);  // futuristic cyan
        case BlockType::METAL:   return glm::vec3(0.75f, 0.75f, 0.80f);  // polished silver
        default:                 return glm::vec3(1.0f,  0.0f,  1.0f);   // magenta = bug
    }
}
