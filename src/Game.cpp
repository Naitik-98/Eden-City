// =============================================================================
// Game.cpp — Main Orchestrator Implementation
// =============================================================================

#include "Game.h"
#include "Config.h"
#include "WorldGenerator.h"
#include "CrystalTowerEntity.h"
#include "WorldSerializer.h"
#include "Debug.h"
#include "TextureManager.h"
#include "AudioManager.h"

#include <GL/freeglut.h>
#include <iostream>
#include <cstdlib>

// -----------------------------------------------------------------------------
// Static instance pointer — needed so GLUT's C callbacks can reach our object.
// There is exactly one Game, created in main(). This is NOT a singleton pattern
// (there's no getInstance()) — it's just a bridge for GLUT's C API.
// -----------------------------------------------------------------------------
Game* Game::s_instance = nullptr;

// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
Game::Game() : m_player(glm::vec3(
    (Config::WORLD_CHUNKS_X * Config::CHUNK_SIZE) / 2.0f,
    (Config::WORLD_CHUNKS_Y * Config::CHUNK_SIZE) + 20.0f,
    (Config::WORLD_CHUNKS_Z * Config::CHUNK_SIZE) + 20.0f)) {
    // Register this instance so static callbacks can delegate to it
    s_instance = this;
    
    // Disable input during intro sequence
    m_player.setInputEnabled(false);
    
    // Initialize the world
    m_world.init();
    
    // Attempt to load saved world; if not found, generate a fresh one
    if (!WorldSerializer::load(&m_world, "saves/world.ecty")) {
        WorldGenerator::generateEdenCity(&m_world);
    }

    // After world generation, we want the player to spawn above the highest block in the middle
    int midX = (Config::WORLD_CHUNKS_X * Config::CHUNK_SIZE) / 2;
    int midZ = (Config::WORLD_CHUNKS_Z * Config::CHUNK_SIZE) / 2;
    int spawnY = Config::WORLD_CHUNKS_Y * Config::CHUNK_SIZE;
    // Find the highest block
    for (int y = spawnY - 1; y >= 0; --y) {
        if (m_world.isSolidAt(midX, y, midZ)) {
            spawnY = y + 2; // spawn a bit above the ground
            break;
        }
    }
    // If no block found, they just spawn high up (which is fine, they fall)
    m_player.setPosition(glm::vec3(midX, spawnY + 20.0f, midZ)); // Fall in cinematic
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

    AudioManager::getInstance().init();

    // --- Register GLUT Callbacks ---
    // These static methods will delegate to our instance methods.
    glutDisplayFunc(s_display);   // called when window needs redrawing
    glutReshapeFunc(s_reshape);   // called when window is resized
    glutIdleFunc(s_idle);         // called when nothing else is happening
    
    // Input callbacks
    glutKeyboardFunc(s_keyboard);
    glutKeyboardUpFunc(s_keyboardUp);
    glutSpecialFunc(s_special);
    glutSpecialUpFunc(s_specialUp);
    
    // Mouse callbacks
    glutPassiveMotionFunc(s_passiveMotion);
    glutMouseFunc(s_mouse);
    
    // Hide and capture cursor
    glutSetCursor(GLUT_CURSOR_NONE);
    glutWarpPointer(Config::WINDOW_WIDTH / 2, Config::WINDOW_HEIGHT / 2);
    InputManager::getInstance().setFirstMouse(true);

    // --- Enter the GLUT Main Loop ---
    // This function DOES NOT RETURN. GLUT takes over and calls our
    // callbacks repeatedly. The application runs until the window is closed.
    std::cout << "[Game] Entering main loop. Close the window to exit.\n";
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutMainLoop();
    
    AudioManager::getInstance().cleanup();
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
    glutPostRedisplay();
}

void Game::s_keyboard(unsigned char key, int x, int y) {
    if (s_instance) s_instance->onKeyboard(key, true);
}

void Game::s_keyboardUp(unsigned char key, int x, int y) {
    if (s_instance) s_instance->onKeyboard(key, false);
}

void Game::s_special(int key, int x, int y) {
    if (s_instance) s_instance->onSpecial(key, true);
}

void Game::s_specialUp(int key, int x, int y) {
    if (s_instance) s_instance->onSpecial(key, false);
}

void Game::s_passiveMotion(int x, int y) {
    if (s_instance) s_instance->onMotion(x, y);
}

void Game::s_mouse(int button, int state, int x, int y) {
    if (s_instance) s_instance->onMouse(button, state, x, y);
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
        
        // Now that OpenGL is initialized, we can load textures
        unsigned int atlasID = TextureManager::getInstance().loadTexture("assets/textures/atlas.png");
        TextureManager::getInstance().bindTexture(atlasID);

        m_gladLoaded = true;
        m_renderer.onResize(m_renderer.getWidth(), m_renderer.getHeight());
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

    InputManager& input = InputManager::getInstance();

    if (m_state == GameState::MENU) {
        m_renderer.beginFrame();
        m_renderer.renderMenu();
        m_renderer.endFrame();
        input.update(); // Clear just-pressed keys/buttons for next frame
        return;
    }

    // --- State Machine ---
    if (m_state == GameState::INTRO_BLACK) {
        m_introTimer -= m_deltaTime;
        if (m_introTimer <= 0.0f) {
            m_state = GameState::INTRO_FALLING;
            m_renderer.setClearColor(Config::SKY_R, Config::SKY_G, Config::SKY_B);
        } else {
            m_renderer.setClearColor(0.0f, 0.0f, 0.0f);
        }
    } else if (m_state == GameState::INTRO_FALLING) {
        if (m_player.isGrounded()) {
            m_state = GameState::PLAYING;
            m_player.setInputEnabled(true);
        }
    }

    // --- Process Input & Logic ---
    m_player.update(m_deltaTime, &m_world, &input);
    m_world.updateEntities(m_deltaTime, &input);
    Debug::getInstance().update(m_deltaTime, &m_player);

    input.update(); // Clear just-pressed keys/buttons for next frame

    // --- Frame Rendering ---
    m_renderer.beginFrame();   
    
    // Only draw the world if we are not in the black screen phase
    if (m_state != GameState::INTRO_BLACK) {
        m_renderer.render(&m_world, m_player.getCamera());
        m_renderer.drawCrosshair(); 
    }
    
    m_renderer.endFrame();     // swap buffers
}

void Game::onReshape(int w, int h) {
    if (!m_gladLoaded) return;
    m_renderer.onResize(w, h);
}

void Game::onIdle() {
    // We update Game logic in onDisplay or we can do it here. 
    // Since delta time is computed in onDisplay, let's keep logic simple.
    // Actually we could just process it in onIdle if we track time there, 
    // but onDisplay does the dt math. Let's move logic to onDisplay, or do it here.
}

void Game::onKeyboard(unsigned char key, bool pressed) {
    // ESC to exit
    if (pressed && key == 27) {
        glutLeaveMainLoop();
    }
    // Enter to start game from menu
    if (pressed && m_state == GameState::MENU && key == 13) {
        m_state = GameState::INTRO_BLACK;
        AudioManager::getInstance().playAmbient("assets/sounds/ambient.wav");
        return;
    }
    // Ctrl+S to save
    if (pressed && key == 19) { // ASCII 19 = Ctrl+S
        WorldSerializer::save(&m_world, "saves/world.ecty");
    }
    InputManager::getInstance().setKeyPressed(key, pressed);
}

void Game::onSpecial(int key, bool pressed) {
    if (pressed) {
        if (key == GLUT_KEY_F3) {
            Debug::getInstance().toggleF3();
        } else if (key == GLUT_KEY_F4) {
            Debug::getInstance().toggleF4();
        }
    }
    InputManager::getInstance().setSpecialKeyPressed(key, pressed);
}

void Game::onMotion(int x, int y) {
    InputManager::getInstance().setMousePos(x, y);
    float dx = InputManager::getInstance().getMouseDeltaX();
    float dy = InputManager::getInstance().getMouseDeltaY();
    
    if (dx != 0 || dy != 0) {
        // Warp pointer back to center to simulate infinite capture
        glutWarpPointer(m_renderer.getWidth() / 2, m_renderer.getHeight() / 2);
        InputManager::getInstance().setFirstMouse(true); // Ignore next jump
    }
}

void Game::onMouse(int button, int state, int x, int y) {
    // GLUT_LEFT_BUTTON = 0, GLUT_MIDDLE_BUTTON = 1, GLUT_RIGHT_BUTTON = 2
    // GLUT_DOWN = 0, GLUT_UP = 1
    InputManager::getInstance().setMouseButton(button, state == 0); // true if down
}
