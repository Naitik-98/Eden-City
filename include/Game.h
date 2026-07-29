// =============================================================================
// Game.h — Main Orchestrator
// =============================================================================
// Game is the central hub that owns all engine subsystems.
// It is created once in main(), then drives the entire application.
//
// Responsibilities:
//   - Register GLUT callbacks (display, reshape, keyboard, mouse, idle)
//   - Calculate delta time each frame
//   - Forward input events to subsystems (InputManager, Player, Debug)
//   - Drive the render loop: Renderer::beginFrame → draw → endFrame
//
// Design: No globals. GLUT callbacks are static methods that call through
// a stored pointer to the single Game instance (s_instance pattern).
// This keeps the GLUT C-API integrated cleanly with our C++ objects.
// =============================================================================

#pragma once

#include "Renderer.h"

class Game {
public:
    // -------------------------------------------------------------------------
    // Constructor/Destructor
    // -------------------------------------------------------------------------
    Game();
    ~Game() = default;

    // Non-copyable (owns unique GL resources)
    Game(const Game&)            = delete;
    Game& operator=(const Game&) = delete;

    // -------------------------------------------------------------------------
    // run() — initialize GLUT, create the window, enter the main loop.
    // This function does not return until the window is closed.
    // argc/argv are forwarded to glutInit as required.
    // -------------------------------------------------------------------------
    void run(int argc, char** argv);

private:
    // -------------------------------------------------------------------------
    // GLUT callback stubs — static so they match the C function pointer
    // signature GLUT expects. They delegate to the instance methods below.
    // -------------------------------------------------------------------------
    static void s_display();
    static void s_reshape(int w, int h);
    static void s_idle();

    // -------------------------------------------------------------------------
    // Per-frame logic (called by s_display / s_idle)
    // -------------------------------------------------------------------------
    void onDisplay();
    void onReshape(int w, int h);
    void onIdle();

    // -------------------------------------------------------------------------
    // Subsystems owned by Game
    // -------------------------------------------------------------------------
    Renderer m_renderer;

    // -------------------------------------------------------------------------
    // Timing
    // -------------------------------------------------------------------------
    float m_lastTime   = 0.0f;   // time of last frame (seconds)
    float m_deltaTime  = 0.0f;   // seconds elapsed since last frame
    bool  m_gladLoaded = false;  // GLAD only loads once after context creation

    // -------------------------------------------------------------------------
    // Singleton pointer — needed so static GLUT callbacks can reach the instance.
    // There is only ever ONE Game object (owned by main()).
    // -------------------------------------------------------------------------
    static Game* s_instance;
};
