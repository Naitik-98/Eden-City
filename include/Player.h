#pragma once

#include "Camera.h"
#include "World.h"
#include "InputManager.h"
#include <glm/glm.hpp>

// A simple Axis-Aligned Bounding Box
struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};

class Player {
public:
    Player(glm::vec3 startPos);

    void update(float dt, World* world, InputManager* input);
    
    Camera* getCamera() { return &m_camera; }
    glm::vec3 getPosition() const { return m_position; }

private:
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    bool m_isGrounded = false;
    
    // Player dimensions
    float m_width = 0.6f;
    float m_height = 1.8f;

    Camera m_camera;

    AABB getAABB(glm::vec3 pos) const;
    bool checkCollision(World* world, const AABB& aabb) const;
    void handleMovement(float dt, World* world, InputManager* input);
};
