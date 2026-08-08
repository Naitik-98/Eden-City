#pragma once

#include "Entity.h"
#include "World.h"
#include <iostream>

class CrystalTowerEntity : public Entity {
public:
    CrystalTowerEntity(glm::vec3 pos) {
        m_position = pos;
        // Set a generous bounding box around the tower's base
        m_aabb.min = pos - glm::vec3(5.0f, 0.0f, 5.0f);
        m_aabb.max = pos + glm::vec3(5.0f, 60.0f, 5.0f);
    }

    void update(float dt, World* world, InputManager* input) override {
        // Static entity — no update logic needed
        (void)dt; (void)world; (void)input;
    }

    void onInteract(Player* player) override {
        if (!m_activated) {
            m_activated = true;
            std::cout << "[Eden City] Crystal Tower at ("
                      << m_position.x << ", " << m_position.y << ", " << m_position.z
                      << ") ACTIVATED!\n"
                      << "[Eden City] The tower resonates with energy...\n";
        } else {
            std::cout << "[Eden City] This tower has already been activated.\n";
        }
    }

    bool isActivated() const { return m_activated; }

private:
    bool m_activated = false;
};
