#pragma once

#include "Entity.h"
#include "Camera.h"
#include "World.h"
#include "InputManager.h"
#include <glm/glm.hpp>

class Player : public Entity {
public:
    Player(glm::vec3 startPos);

    void update(float dt, World* world, InputManager* input) override;
    
    Camera* getCamera() { return &m_camera; }

    void setInputEnabled(bool enabled) { m_inputEnabled = enabled; }
    bool isGrounded() const { return m_isGrounded; }

private:
    glm::vec3 m_velocity;
    bool m_isGrounded = false;
    bool m_inputEnabled = true;
    
    // Interaction state
    bool m_wasInteractPressed = false;
    
    // Player dimensions
    float m_width = 0.6f;
    float m_height = 1.8f;

    Camera m_camera;

    AABB getAABB(glm::vec3 pos) const;
    bool checkCollision(World* world, const AABB& aabb) const;
    void handleMovement(float dt, World* world, InputManager* input);
};
