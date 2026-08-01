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

void InputManager::setMouseButton(int button, bool state) {
    if (button >= 0 && button < 5) {
        if (state && !m_mouseButtons[button]) {
            m_mouseButtonsJustPressed[button] = true;
        }
        m_mouseButtons[button] = state;
    }
}

bool InputManager::isMouseButtonPressed(int button) const {
    if (button >= 0 && button < 5) return m_mouseButtons[button];
    return false;
}

bool InputManager::wasMouseButtonJustPressed(int button) {
    if (button >= 0 && button < 5) return m_mouseButtonsJustPressed[button];
    return false;
}

void InputManager::update() {
    // Clear just-pressed state every frame
    for (int i = 0; i < 5; ++i) {
        m_mouseButtonsJustPressed[i] = false;
    }
}
