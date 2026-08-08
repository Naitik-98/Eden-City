#pragma once

#include <glm/glm.hpp>

class World;
class InputManager;
class Player;

// A simple Axis-Aligned Bounding Box
struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};

class Entity {
public:
    virtual ~Entity() = default;
    
    virtual void update(float dt, World* world, InputManager* input) = 0;
    virtual void onInteract(Player* player) {}
    
    glm::vec3 getPosition() const { return m_position; }
    void setPosition(glm::vec3 pos) { m_position = pos; }
    
    AABB getBoundingBox() const { return m_aabb; }
    void setBoundingBox(const AABB& aabb) { m_aabb = aabb; }

protected:
    glm::vec3 m_position{0.0f, 0.0f, 0.0f};
    AABB m_aabb{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}};
};
