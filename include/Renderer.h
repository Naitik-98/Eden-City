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

#include "ShaderProgram.h"
#include "World.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

class Camera;
class Chunk;

// A single frustum plane (normal + distance from origin)
struct Plane {
    glm::vec3 normal;
    float     distance;
};

class Renderer {
public:
    ~Renderer();

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
    // setClearColor() — override the background clear color dynamically
    // -------------------------------------------------------------------------
    void setClearColor(float r, float g, float b);

    // -------------------------------------------------------------------------
    // onResize() — call when the window is resized (GLUT reshape callback).
    // Updates the OpenGL viewport to match the new window dimensions.
    // -------------------------------------------------------------------------
    void onResize(int width, int height);

    // -------------------------------------------------------------------------
    // render() — renders the 3D world
    // -------------------------------------------------------------------------
    void render(World* world, Camera* camera);

    // -------------------------------------------------------------------------
    // drawCrosshair() — renders a 2D crosshair overlay
    // -------------------------------------------------------------------------
    void drawCrosshair();

    // -------------------------------------------------------------------------
    // renderSky() — renders the procedural gradient sky
    // -------------------------------------------------------------------------
    void renderSky(Camera* camera);

    // -------------------------------------------------------------------------
    // renderMenu() — renders the main menu screen
    // -------------------------------------------------------------------------
    void renderMenu();

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

private:
    ShaderProgram m_shader;
    ShaderProgram m_skyShader;
    ShaderProgram m_crosshairShader;
    ShaderProgram m_menuShader;
    
    int m_width = 1280;
    int m_height = 720;
    
    float m_clearR = 0.05f;
    float m_clearG = 0.05f;
    float m_clearB = 0.12f;
    
    // Crosshair resources
    unsigned int m_crosshairVAO = 0;
    unsigned int m_crosshairVBO = 0;
    
    // Sky resources
    unsigned int m_skyVAO = 0;
    unsigned int m_skyVBO = 0;

    // Menu resources
    unsigned int m_menuTextureID = 0;
    unsigned int m_menuVAO = 0;
    unsigned int m_menuVBO = 0;

    // -------------------------------------------------------------------------
    // Frustum culling helpers
    // -------------------------------------------------------------------------
    // Extract 6 frustum planes from a combined projection*view matrix.
    static void extractFrustumPlanes(const glm::mat4& vp, Plane planes[6]);
    // Returns true if the chunk's AABB is (at least partly) inside the frustum.
    static bool isChunkVisible(const Chunk* chunk, const Plane planes[6]);
};

