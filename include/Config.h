// =============================================================================
// Config.h — Eden City Global Constants
// =============================================================================
// Every tunable value lives here. No magic numbers in source files.
// Grouped by subsystem for easy navigation.
// =============================================================================

#pragma once

namespace Config {

    // -------------------------------------------------------------------------
    // Window
    // -------------------------------------------------------------------------
    constexpr int   WINDOW_WIDTH  = 1280;
    constexpr int   WINDOW_HEIGHT = 720;
    constexpr const char* WINDOW_TITLE = "Eden City";

    // -------------------------------------------------------------------------
    // World
    // -------------------------------------------------------------------------
    constexpr int   CHUNK_SIZE    = 16;         // blocks per chunk axis
    constexpr int   WORLD_CHUNKS_X = 4;         // chunks along X
    constexpr int   WORLD_CHUNKS_Y = 8;         // chunks along Y (vertical)
    constexpr int   WORLD_CHUNKS_Z = 4;         // chunks along Z
    // Total world: 64 x 128 x 64 blocks

    // -------------------------------------------------------------------------
    // Player
    // -------------------------------------------------------------------------
    constexpr float PLAYER_WIDTH  = 0.6f;       // bounding box width (X and Z)
    constexpr float PLAYER_HEIGHT = 1.8f;       // bounding box height (Y)
    constexpr float PLAYER_EYE_OFFSET = 1.6f;   // camera sits slightly below top
    constexpr float PLAYER_SPEED  = 5.0f;       // blocks per second
    constexpr float PLAYER_SPRINT_MULTIPLIER = 1.6f;

    // -------------------------------------------------------------------------
    // Physics
    // -------------------------------------------------------------------------
    constexpr float GRAVITY           = 20.0f;  // blocks/s^2 downward
    constexpr float JUMP_FORCE        = 8.0f;   // initial upward velocity
    constexpr float TERMINAL_VELOCITY = 50.0f;  // max falling speed

    // -------------------------------------------------------------------------
    // Camera
    // -------------------------------------------------------------------------
    constexpr float MOUSE_SENSITIVITY = 0.1f;   // degrees per pixel of mouse movement
    constexpr float FOV               = 70.0f;  // vertical field of view in degrees
    constexpr float NEAR_CLIP         = 0.1f;
    constexpr float FAR_CLIP          = 300.0f;
    constexpr float PITCH_LIMIT       = 89.0f;  // clamp to prevent gimbal lock

    // -------------------------------------------------------------------------
    // Rendering
    // -------------------------------------------------------------------------
    // Sky color (dark blue-gray, futuristic feel)
    constexpr float SKY_R = 0.05f;
    constexpr float SKY_G = 0.05f;
    constexpr float SKY_B = 0.12f;

} // namespace Config
