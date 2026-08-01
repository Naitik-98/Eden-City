#include "InputManager.h"
#include <cctype>

void InputManager::setKeyPressed(unsigned char key, bool pressed) {
    // Convert to lowercase to handle 'W' vs 'w' simply if needed, but 
    // it's often better to just store what's given. We'll store exact key.
    // For safety, clamp to 0-255
    m_keys[key] = pressed;
    if (std::isalpha(key)) {
        m_keys[std::tolower(key)] = pressed;
        m_keys[std::toupper(key)] = pressed;
    }
}

void InputManager::setSpecialKeyPressed(int key, bool pressed) {
    if (key >= 0 && key < 256) {
        m_specialKeys[key] = pressed;
    }
}

bool InputManager::isKeyPressed(unsigned char key) const {
    return m_keys[key];
}

bool InputManager::isSpecialKeyPressed(int key) const {
    if (key >= 0 && key < 256) {
        return m_specialKeys[key];
    }
    return false;
}

void InputManager::setMousePos(int x, int y) {
    if (m_firstMouse) {
        m_mouseX = x;
        m_mouseY = y;
        m_firstMouse = false;
        return;
    }

    m_deltaX = x - m_mouseX;
    m_deltaY = m_mouseY - y; // Reversed since y-coordinates go from bottom to top in OpenGL, but GLUT is top to bottom. Actually, GLUT is top to bottom, so y increases downwards. Camera pitch usually expects positive = looking up. Wait, let's keep standard math.
    // We will handle the orientation in Camera.
    m_deltaY = y - m_mouseY; 

    m_mouseX = x;
    m_mouseY = y;
}

void InputManager::clearMouseDelta() {
    m_deltaX = 0;
    m_deltaY = 0;
}
