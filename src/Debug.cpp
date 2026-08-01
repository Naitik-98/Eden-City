#include "Debug.h"
#include "Config.h"
#include <glad/glad.h>
#include <GL/freeglut.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

void Debug::update(float dt, Player* player) {
    // Calculate FPS
    m_frameCount++;
    m_fpsTimer += dt;
    if (m_fpsTimer >= 1.0f) {
        m_currentFps = m_frameCount;
        m_frameCount = 0;
        m_fpsTimer -= 1.0f;
    }

    if (m_showF3) {
        // Use Window Title as our debug overlay, since OpenGL 3.3 Core 
        // does not support legacy glutBitmapCharacter text rendering easily.
        std::ostringstream ss;
        glm::vec3 pos = player->getPosition();
        
        int chunkX = static_cast<int>(std::floor(pos.x / Config::CHUNK_SIZE));
        int chunkY = static_cast<int>(std::floor(pos.y / Config::CHUNK_SIZE));
        int chunkZ = static_cast<int>(std::floor(pos.z / Config::CHUNK_SIZE));

        ss << Config::WINDOW_TITLE << " | "
           << "FPS: " << m_currentFps << " | "
           << "Pos: (" << std::fixed << std::setprecision(1) 
           << pos.x << ", " << pos.y << ", " << pos.z << ") | "
           << "Chunk: (" << chunkX << ", " << chunkY << ", " << chunkZ << ")";
           
        glutSetWindowTitle(ss.str().c_str());
    }
}

void Debug::toggleF3() {
    m_showF3 = !m_showF3;
    if (!m_showF3) {
        // Reset window title
        glutSetWindowTitle(Config::WINDOW_TITLE);
    }
    std::cout << "[Debug] F3 overlay: " << (m_showF3 ? "ON" : "OFF") << "\n";
}

void Debug::toggleF4() {
    m_showF4 = !m_showF4;
    if (m_showF4) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    std::cout << "[Debug] Wireframe mode: " << (m_showF4 ? "ON" : "OFF") << "\n";
}
