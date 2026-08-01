// =============================================================================
// Renderer.cpp — OpenGL Rendering Layer Implementation
// =============================================================================

#include "Renderer.h"
#include "Config.h"
#include "Camera.h"

// GLAD must be included before any other OpenGL headers.
// It defines all OpenGL 3.3 Core function pointers.
#include <glad/glad.h>

// freeglut — provides glutSwapBuffers() and the GL context
#include <GL/freeglut.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MeshBuilder.h"

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

    if (!m_shader.load("assets/shaders/basic.vert", "assets/shaders/basic.frag")) {
        std::cerr << "[Renderer] ERROR: Failed to load basic shaders.\n";
        return false;
    }

    // Set up crosshair geometry (two lines forming a +)
    // Vertices format: x, y, z, r, g, b
    float crosshairVertices[] = {
        // Horizontal line
        -10.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
         10.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
        // Vertical line
         0.0f, -10.0f, 0.0f,  1.0f, 1.0f, 1.0f,
         0.0f,  10.0f, 0.0f,  1.0f, 1.0f, 1.0f
    };

    glGenVertexArrays(1, &m_crosshairVAO);
    glGenBuffers(1, &m_crosshairVBO);

    glBindVertexArray(m_crosshairVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_crosshairVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

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

    m_width = width;
    m_height = height;

    // Tell OpenGL the new pixel dimensions to render into.
    // (0,0) is bottom-left corner, which is OpenGL's coordinate origin.
    glViewport(0, 0, width, height);
}

// -----------------------------------------------------------------------------
// render() — renders the 3D world
// -----------------------------------------------------------------------------
void Renderer::render(World* world, Camera* camera) {
    if (!world || !camera) return;

    m_shader.bind();

    glm::mat4 model         = glm::mat4(1.0f);
    glm::mat4 view          = camera->getViewMatrix();
    glm::mat4 projection    = glm::perspective(glm::radians(camera->Zoom), (float)m_width / (float)m_height, Config::NEAR_CLIP, Config::FAR_CLIP);

    // Calculate MVP
    glm::mat4 mvp = projection * view * model;

    // Pass MVP to shader
    m_shader.setMat4("MVP", mvp);

    // Render all chunks
    for (const auto& chunk : world->getChunks()) {
        if (chunk->isDirty()) {
            MeshBuilder::buildMesh(chunk.get(), world);
        }

        if (chunk->vertexCount > 0 && chunk->VAO != 0) {
            glBindVertexArray(chunk->VAO);
            glDrawArrays(GL_TRIANGLES, 0, chunk->vertexCount);
        }
    }
    
    glBindVertexArray(0);
}

// -----------------------------------------------------------------------------
// drawCrosshair() — overlay rendering
// -----------------------------------------------------------------------------
void Renderer::drawCrosshair() {
    // Disable depth testing to draw over everything
    glDisable(GL_DEPTH_TEST);
    
    m_shader.bind();

    // Orthographic projection matching screen pixels
    glm::mat4 projection = glm::ortho(0.0f, (float)m_width, 0.0f, (float)m_height);
    
    // Model matrix to center the crosshair
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3((float)m_width / 2.0f, (float)m_height / 2.0f, 0.0f));
    
    // View matrix is identity
    glm::mat4 view = glm::mat4(1.0f);

    glm::mat4 mvp = projection * view * model;
    m_shader.setMat4("MVP", mvp);

    glBindVertexArray(m_crosshairVAO);
    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0);

    // Re-enable depth testing for next 3D frame
    glEnable(GL_DEPTH_TEST);
}
