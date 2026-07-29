// =============================================================================
// Renderer.h — OpenGL Rendering Layer
// =============================================================================
// The Renderer is the ONLY class that calls OpenGL directly (besides
// ShaderProgram and MeshBuilder which are rendering helpers).
//
// Responsibilities (Week 1, Step 2):
//   - Initialize GLAD (load all OpenGL 3.3 Core function pointers)
//   - Clear the framebuffer to the sky color each frame
//
// Will grow each step:
//   Step 3: draw a cube via VBO/VAO
//   Step 4: draw voxel chunks
//   Step 8: draw crosshair HUD
// =============================================================================

#pragma once

class Renderer {
public:
    // -------------------------------------------------------------------------
    // init() — call once AFTER the OpenGL context has been created by GLUT.
    // Loads all OpenGL function pointers via GLAD.
    // Returns false if GLAD fails (fatal error).
    // -------------------------------------------------------------------------
    bool init();

    // -------------------------------------------------------------------------
    // beginFrame() — call at the start of every display callback.
    // Clears color + depth buffers to the sky color defined in Config.h.
    // -------------------------------------------------------------------------
    void beginFrame();

    // -------------------------------------------------------------------------
    // endFrame() — call at the end of every display callback.
    // Swaps front/back buffers via glutSwapBuffers().
    // -------------------------------------------------------------------------
    void endFrame();

    // -------------------------------------------------------------------------
    // onResize() — call when the window is resized (GLUT reshape callback).
    // Updates the OpenGL viewport to match the new window dimensions.
    // -------------------------------------------------------------------------
    void onResize(int width, int height);
};
