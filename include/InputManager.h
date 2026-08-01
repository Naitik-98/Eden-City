#pragma once

#include <array>

class InputManager {
public:
    static InputManager& getInstance() {
        static InputManager instance;
        return instance;
    }

    // Keyboard
    void setKeyPressed(unsigned char key, bool pressed);
    void setSpecialKeyPressed(int key, bool pressed);
    
    bool isKeyPressed(unsigned char key) const;
    bool isSpecialKeyPressed(int key) const;

    // Mouse
    void setMousePos(int x, int y);
    int getMouseX() const { return m_mouseX; }
    int getMouseY() const { return m_mouseY; }
    int getMouseDeltaX() const { return m_deltaX; }
    int getMouseDeltaY() const { return m_deltaY; }
    
    void clearMouseDelta();
    void setFirstMouse(bool first) { m_firstMouse = first; }
    bool isFirstMouse() const { return m_firstMouse; }

private:
    InputManager() = default;

    std::array<bool, 256> m_keys{false};
    std::array<bool, 256> m_specialKeys{false};

    int m_mouseX = 0;
    int m_mouseY = 0;
    int m_deltaX = 0;
    int m_deltaY = 0;
    
    bool m_firstMouse = true;
};
