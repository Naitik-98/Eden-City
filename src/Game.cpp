// =============================================================================
// Game.cpp — Main Orchestrator Implementation
// =============================================================================

#include "Game.h"
#include "Config.h"

#include <GL/freeglut.h>
#include <iostream>

// -----------------------------------------------------------------------------
// Static instance pointer — needed so GLUT's C callbacks can reach our object.
// There is exactly one Game, created in main(). This is NOT a singleton pattern
// (there's no getInstance()) — it's just a bridge for GLUT's C API.
// -----------------------------------------------------------------------------
Game* Game::s_instance = nullptr;

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
Game::Game() {
    // Register this instance so static callbacks can delegate to it
    s_instance = this;
}

// -----------------------------------------------------------------------------
// run() — set up GLUT, create window, enter main loop
// -----------------------------------------------------------------------------
void Game::run(int argc, char** argv) {
    // --- GLUT Initialization ---
    // glutInit parses any GLUT-specific command-line arguments.
    glutInit(&argc, argv);

    // Request a double-buffered, RGBA, depth-buffered window.
    //   GLUT_DOUBLE    → double buffering (no flickering)
    //   GLUT_RGBA      → standard 32-bit color
    //   GLUT_DEPTH     → 24-bit depth buffer (needed for 3D from Step 3)
    //   GLUT_3_2_CORE_PROFILE is freeglut-specific for Core context
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    // Request OpenGL 3.3 Core profile context.
    // Core profile removes all deprecated legacy features (glBegin, etc.)
    // This forces us to use modern shaders + VAO/VBO correctly.
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    // Set window size and position (centred-ish on screen)
    glutInitWindowSize(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);

    // Create the window — this also creates the OpenGL context.
    // GLAD is NOT loaded yet (we need the context to exist first).
    glutCreateWindow(Config::WINDOW_TITLE);

    // --- Register GLUT Callbacks ---
    // These static methods will delegate to our instance methods.
    glutDisplayFunc(s_display);   // called when window needs redrawing
    glutReshapeFunc(s_reshape);   // called when window is resized
    glutIdleFunc(s_idle);         // called when nothing else is happening

    // --- Enter the GLUT Main Loop ---
    // This function DOES NOT RETURN. GLUT takes over and calls our
    // callbacks repeatedly. The application runs until the window is closed.
    std::cout << "[Game] Entering main loop. Close the window to exit.\n";
    glutMainLoop();
}

// =============================================================================
// Static GLUT Callback Stubs
// These have the exact signature GLUT expects (C function pointers).
// They simply call through to the instance methods on s_instance.
// =============================================================================

void Game::s_display() {
    if (s_instance) s_instance->onDisplay();
}

void Game::s_reshape(int w, int h) {
    if (s_instance) s_instance->onReshape(w, h);
}

void Game::s_idle() {
    // Force a redraw every idle tick — keeps the loop running smoothly.
    // glutPostRedisplay() marks the window as needing a redraw, which
    // triggers s_display() on the next GLUT iteration.
    glutPostRedisplay();
}

// =============================================================================
// Instance Callback Implementations
// =============================================================================

void Game::onDisplay() {
    // --- First-time GLAD initialization ---
    // GLAD must be initialized after the OpenGL context exists.
    // The first call to onDisplay() is guaranteed to have a valid context.
    if (!m_gladLoaded) {
        if (!m_renderer.init()) {
            std::cerr << "[Game] FATAL: Renderer failed to initialize. Exiting.\n";
            glutLeaveMainLoop();  // cleanly exit GLUT loop
            return;
        }
        m_gladLoaded = true;
    }

    // --- Delta Time Calculation ---
    // glutGet(GLUT_ELAPSED_TIME) returns milliseconds since glutInit().
    // We convert to seconds for physics and movement calculations.
    float currentTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) / 1000.0f;
    m_deltaTime  = currentTime - m_lastTime;
    m_lastTime   = currentTime;

    // Clamp delta time: if the app was paused/minimized, dt could be huge.
    // Clamping to 100ms prevents physics explosions on resume.
    if (m_deltaTime > 0.1f) m_deltaTime = 0.1f;

    // --- Frame Rendering ---
    m_renderer.beginFrame();   // clear to sky color
    // (Step 3 will add: m_renderer.renderWorld(...))
    // (Step 8 will add: m_renderer.drawCrosshair())
    m_renderer.endFrame();     // swap buffers
}

void Game::onReshape(int w, int h) {
    m_renderer.onResize(w, h);
}

void Game::onIdle() {
    // Idle logic (unused in Step 2).
    // Step 3+: could update game state here instead of in onDisplay.
}
