// =============================================================================
// Renderer.cpp — OpenGL Rendering Layer Implementation
// =============================================================================

#include "Renderer.h"
#include "Config.h"

// GLAD must be included before any other OpenGL headers.
// It defines all OpenGL 3.3 Core function pointers.
#include <glad/glad.h>

// freeglut — provides glutSwapBuffers() and the GL context
#include <GL/freeglut.h>

#include <iostream>

// -----------------------------------------------------------------------------
// init() — called once after GLUT creates the GL context
// -----------------------------------------------------------------------------
bool Renderer::init() {
    // Load all OpenGL 3.3 Core function pointers.
    // glutGetProcAddress is the loader function freeglut provides.
    // gladLoadGLLoader returns 0 on failure.
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glutGetProcAddress))) {
        std::cerr << "[Renderer] ERROR: Failed to initialize GLAD.\n";
        std::cerr << "           Make sure your GPU supports OpenGL 3.3+.\n";
        return false;
    }

    // Log OpenGL version so we know what the driver gave us
    std::cout << "[Renderer] OpenGL Version : "
              << glGetString(GL_VERSION) << "\n";
    std::cout << "[Renderer] GLSL Version   : "
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    std::cout << "[Renderer] Renderer       : "
              << glGetString(GL_RENDERER) << "\n";

    // Enable depth testing — fragments behind others are discarded.
    // Essential for 3D rendering correctness (added now, used from Step 3).
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);   // standard: keep the fragment closest to camera

    return true;
}

// -----------------------------------------------------------------------------
// beginFrame() — clear buffers at start of each frame
// -----------------------------------------------------------------------------
void Renderer::beginFrame() {
    // Set the sky color and clear both the color and depth buffers.
    // Color buffer: paints every pixel with sky color before drawing geometry.
    // Depth buffer: resets depth values so new frame starts fresh.
    glClearColor(Config::SKY_R, Config::SKY_G, Config::SKY_B, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// -----------------------------------------------------------------------------
// endFrame() — swap buffers (double buffering)
// -----------------------------------------------------------------------------
void Renderer::endFrame() {
    // Double buffering: we draw to the back buffer, then swap it to front.
    // This prevents flickering — the user always sees a complete frame.
    glutSwapBuffers();
}

// -----------------------------------------------------------------------------
// onResize() — update viewport when window is resized
// -----------------------------------------------------------------------------
void Renderer::onResize(int width, int height) {
    // Guard against zero-size window (minimize edge case)
    if (height == 0) height = 1;

    // Tell OpenGL the new pixel dimensions to render into.
    // (0,0) is bottom-left corner, which is OpenGL's coordinate origin.
    glViewport(0, 0, width, height);
}
