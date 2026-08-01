#pragma once

#include "Player.h"

class Debug {
public:
    static Debug& getInstance() {
        static Debug instance;
        return instance;
    }

    void update(float dt, Player* player);
    
    void toggleF3();
    void toggleF4();

    bool isWireframe() const { return m_showF4; }

private:
    Debug() = default;

    bool m_showF3 = false;
    bool m_showF4 = false;

    // FPS tracking
    int m_frameCount = 0;
    float m_fpsTimer = 0.0f;
    int m_currentFps = 0;
};
