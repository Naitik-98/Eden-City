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
#include "TextureManager.h"

// -----------------------------------------------------------------------------
// Destructor — clean up OpenGL resources
// -----------------------------------------------------------------------------
Renderer::~Renderer() {
    if (m_crosshairVAO != 0) glDeleteVertexArrays(1, &m_crosshairVAO);
    if (m_crosshairVBO != 0) glDeleteBuffers(1, &m_crosshairVBO);
    
    if (m_skyVAO != 0) glDeleteVertexArrays(1, &m_skyVAO);
    if (m_skyVBO != 0) glDeleteBuffers(1, &m_skyVBO);
    
    if (m_menuVAO != 0) glDeleteVertexArrays(1, &m_menuVAO);
    if (m_menuVBO != 0) glDeleteBuffers(1, &m_menuVBO);
    
    if (m_menuTextureID != 0) glDeleteTextures(1, &m_menuTextureID);
}

// -----------------------------------------------------------------------------
// init() — called once after GLUT creates the GL context
// -----------------------------------------------------------------------------
bool Renderer::init() {
    // Load all OpenGL 3.3 Core function pointers.
    // glutGetProcAddress is the loader function freeglut provides.
    // gladLoadGLLoader returns 0 on failure.
    if (!gladLoadGL()) {
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
    // Vertices format: x, y, z only (3 floats each)
    float crosshairVertices[] = {
        // Horizontal line
        -10.0f, 0.0f, 0.0f,
         10.0f, 0.0f, 0.0f,
        // Vertical line
         0.0f, -10.0f, 0.0f,
         0.0f,  10.0f, 0.0f
    };

    glGenVertexArrays(1, &m_crosshairVAO);
    glGenBuffers(1, &m_crosshairVBO);

    glBindVertexArray(m_crosshairVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_crosshairVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);

    // Position attribute only (location 0, 3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    if (!m_crosshairShader.load("assets/shaders/crosshair.vert", "assets/shaders/crosshair.frag")) {
        std::cerr << "[Renderer] ERROR: Failed to load crosshair shaders.\n";
        return false;
    }

    if (!m_menuShader.load("assets/shaders/menu.vert", "assets/shaders/menu.frag")) {
        std::cerr << "[Renderer] ERROR: Failed to load menu shaders.\n";
        return false;
    }

    // Set up menu quad
    float menuVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &m_menuVAO);
    glGenBuffers(1, &m_menuVBO);
    glBindVertexArray(m_menuVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_menuVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(menuVertices), menuVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Load menu texture
    m_menuTextureID = TextureManager::getInstance().loadTexture("assets/textures/menu.png");

    if (!m_skyShader.load("assets/shaders/sky.vert", "assets/shaders/sky.frag")) {
        std::cerr << "[Renderer] ERROR: Failed to load sky shaders.\n";
        return false;
    }

    float skyQuadVertices[] = {
        -1.0f,  1.0f,
        -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f
    };

    glGenVertexArrays(1, &m_skyVAO);
    glGenBuffers(1, &m_skyVBO);
    glBindVertexArray(m_skyVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyQuadVertices), skyQuadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
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
    glClearColor(m_clearR, m_clearG, m_clearB, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setClearColor(float r, float g, float b) {
    m_clearR = r;
    m_clearG = g;
    m_clearB = b;
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

    glm::mat4 model         = glm::mat4(1.0f);
    glm::mat4 view          = camera->getViewMatrix();
    glm::mat4 projection    = glm::perspective(glm::radians(camera->Zoom), (float)m_width / (float)m_height, Config::NEAR_CLIP, Config::FAR_CLIP);

    // Draw the sky background
    renderSky(camera);
    
    m_shader.bind();

    // Calculate MVP
    glm::mat4 mvp = projection * view * model;

    // Pass MVP to shader
    m_shader.setMat4("MVP", mvp);

    // Pass Fog parameters
    m_shader.setVec3("viewPos", camera->Position);
    m_shader.setVec3("fogColor", glm::vec3(m_clearR, m_clearG, m_clearB));

    Plane frustumPlanes[6];
    extractFrustumPlanes(projection * view, frustumPlanes);

    // Render all chunks
    for (const auto& chunk : world->getChunks()) {
        if (!isChunkVisible(chunk.get(), frustumPlanes)) {
            continue;
        }

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
    
    m_crosshairShader.bind();

    // Orthographic projection matching screen pixels
    glm::mat4 projection = glm::ortho(0.0f, (float)m_width, 0.0f, (float)m_height);
    
    // Model matrix to center the crosshair
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3((float)m_width / 2.0f, (float)m_height / 2.0f, 0.0f));
    
    // View matrix is identity
    glm::mat4 view = glm::mat4(1.0f);

    glm::mat4 mvp = projection * view * model;
    m_crosshairShader.setMat4("MVP", mvp);
    m_crosshairShader.setVec4("color", glm::vec4(1.0f, 1.0f, 1.0f, 0.8f));

    glBindVertexArray(m_crosshairVAO);
    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0);

    // Re-enable depth testing for next 3D frame
    glEnable(GL_DEPTH_TEST);
}

// -----------------------------------------------------------------------------
// renderSky() — renders the procedural gradient sky
// -----------------------------------------------------------------------------
void Renderer::renderSky(Camera* camera) {
    glDepthMask(GL_FALSE); // Don't write to depth buffer
    
    m_skyShader.bind();
    
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)m_width / (float)m_height, Config::NEAR_CLIP, Config::FAR_CLIP);
    
    m_skyShader.setMat4("invProjection", glm::inverse(projection));
    // Remove translation from view matrix so the sky doesn't move when camera moves
    glm::mat4 viewNoTranslation = glm::mat4(glm::mat3(view));
    m_skyShader.setMat4("invView", glm::inverse(viewNoTranslation));
    
    // Pass colors. Horizon matches fog. Zenith can be a brighter blue.
    m_skyShader.setVec3("horizonColor", glm::vec3(m_clearR, m_clearG, m_clearB));
    m_skyShader.setVec3("zenithColor", glm::vec3(m_clearR * 1.5f, m_clearG * 2.0f, m_clearB * 3.0f));
    
    glBindVertexArray(m_skyVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    
    glDepthMask(GL_TRUE); // Re-enable depth writing
}

// -----------------------------------------------------------------------------
// renderMenu() — renders the main menu screen
// -----------------------------------------------------------------------------
void Renderer::renderMenu() {
    glDisable(GL_DEPTH_TEST);
    m_menuShader.bind();
    
    // Bind menu texture to GL_TEXTURE0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_menuTextureID);
    m_menuShader.setInt("menuTexture", 0);

    glBindVertexArray(m_menuVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
}

// -----------------------------------------------------------------------------
// Frustum Culling Helpers
// -----------------------------------------------------------------------------
void Renderer::extractFrustumPlanes(const glm::mat4& vp, Plane planes[6]) {
    // Left
    planes[0].normal.x = vp[0][3] + vp[0][0];
    planes[0].normal.y = vp[1][3] + vp[1][0];
    planes[0].normal.z = vp[2][3] + vp[2][0];
    planes[0].distance = vp[3][3] + vp[3][0];
    // Right
    planes[1].normal.x = vp[0][3] - vp[0][0];
    planes[1].normal.y = vp[1][3] - vp[1][0];
    planes[1].normal.z = vp[2][3] - vp[2][0];
    planes[1].distance = vp[3][3] - vp[3][0];
    // Bottom
    planes[2].normal.x = vp[0][3] + vp[0][1];
    planes[2].normal.y = vp[1][3] + vp[1][1];
    planes[2].normal.z = vp[2][3] + vp[2][1];
    planes[2].distance = vp[3][3] + vp[3][1];
    // Top
    planes[3].normal.x = vp[0][3] - vp[0][1];
    planes[3].normal.y = vp[1][3] - vp[1][1];
    planes[3].normal.z = vp[2][3] - vp[2][1];
    planes[3].distance = vp[3][3] - vp[3][1];
    // Near
    planes[4].normal.x = vp[0][3] + vp[0][2];
    planes[4].normal.y = vp[1][3] + vp[1][2];
    planes[4].normal.z = vp[2][3] + vp[2][2];
    planes[4].distance = vp[3][3] + vp[3][2];
    // Far
    planes[5].normal.x = vp[0][3] - vp[0][2];
    planes[5].normal.y = vp[1][3] - vp[1][2];
    planes[5].normal.z = vp[2][3] - vp[2][2];
    planes[5].distance = vp[3][3] - vp[3][2];

    for (int i = 0; i < 6; ++i) {
        float length = glm::length(planes[i].normal);
        planes[i].normal /= length;
        planes[i].distance /= length;
    }
}

bool Renderer::isChunkVisible(const Chunk* chunk, const Plane planes[6]) {
    glm::vec3 min(chunk->getX() * Config::CHUNK_SIZE, chunk->getY() * Config::CHUNK_SIZE, chunk->getZ() * Config::CHUNK_SIZE);
    glm::vec3 max = min + glm::vec3(Config::CHUNK_SIZE, Config::CHUNK_SIZE, Config::CHUNK_SIZE);

    for (int i = 0; i < 6; ++i) {
        int out = 0;
        out += ((glm::dot(planes[i].normal, glm::vec3(min.x, min.y, min.z)) + planes[i].distance < 0.0f) ? 1 : 0);
        out += ((glm::dot(planes[i].normal, glm::vec3(max.x, min.y, min.z)) + planes[i].distance < 0.0f) ? 1 : 0);
        out += ((glm::dot(planes[i].normal, glm::vec3(min.x, max.y, min.z)) + planes[i].distance < 0.0f) ? 1 : 0);
        out += ((glm::dot(planes[i].normal, glm::vec3(max.x, max.y, min.z)) + planes[i].distance < 0.0f) ? 1 : 0);
        out += ((glm::dot(planes[i].normal, glm::vec3(min.x, min.y, max.z)) + planes[i].distance < 0.0f) ? 1 : 0);
        out += ((glm::dot(planes[i].normal, glm::vec3(max.x, min.y, max.z)) + planes[i].distance < 0.0f) ? 1 : 0);
        out += ((glm::dot(planes[i].normal, glm::vec3(min.x, max.y, max.z)) + planes[i].distance < 0.0f) ? 1 : 0);
        out += ((glm::dot(planes[i].normal, glm::vec3(max.x, max.y, max.z)) + planes[i].distance < 0.0f) ? 1 : 0);
        if (out == 8) return false;
    }
    return true;
}
