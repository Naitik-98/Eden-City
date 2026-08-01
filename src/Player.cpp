#include "Player.h"
#include "Config.h"
#include <cmath>

Player::Player(glm::vec3 startPos) : m_position(startPos), m_velocity(0.0f) {
    m_camera.Position = m_position + glm::vec3(0.0f, m_height * 0.9f, 0.0f); // Eye level
}

AABB Player::getAABB(glm::vec3 pos) const {
    // AABB centered horizontally, extending upwards from pos
    AABB box;
    box.min = pos - glm::vec3(m_width / 2.0f, 0.0f, m_width / 2.0f);
    box.max = pos + glm::vec3(m_width / 2.0f, m_height, m_width / 2.0f);
    return box;
}

bool Player::checkCollision(World* world, const AABB& aabb) const {
    // Check all block coordinates that intersect with the AABB
    int minX = std::floor(aabb.min.x);
    int maxX = std::floor(aabb.max.x);
    int minY = std::floor(aabb.min.y);
    int maxY = std::floor(aabb.max.y);
    int minZ = std::floor(aabb.min.z);
    int maxZ = std::floor(aabb.max.z);

    for (int x = minX; x <= maxX; ++x) {
        for (int y = minY; y <= maxY; ++y) {
            for (int z = minZ; z <= maxZ; ++z) {
                if (world->isSolidAt(x, y, z)) {
                    return true;
                }
            }
        }
    }
    return false;
}

void Player::handleMovement(float dt, World* world, InputManager* input) {
    // Temporary camera used to figure out direction vectors
    float moveSpeed = 5.0f * dt;
    glm::vec3 moveDir(0.0f);

    if (input->isKeyPressed('w')) moveDir += m_camera.Front;
    if (input->isKeyPressed('s')) moveDir -= m_camera.Front;
    if (input->isKeyPressed('a')) moveDir -= m_camera.Right;
    if (input->isKeyPressed('d')) moveDir += m_camera.Right;

    // Flatten movement for walking
    moveDir.y = 0.0f;
    if (glm::length(moveDir) > 0.01f) {
        moveDir = glm::normalize(moveDir) * moveSpeed;
    }
    
    // Gravity & Jumping
    const float GRAVITY = 25.0f;
    const float JUMP_IMPULSE = 8.5f;
    const float TERMINAL_VELOCITY = -50.0f;

    m_velocity.y -= GRAVITY * dt;
    if (m_velocity.y < TERMINAL_VELOCITY) {
        m_velocity.y = TERMINAL_VELOCITY;
    }

    if (m_isGrounded && input->isKeyPressed(' ')) {
        m_velocity.y = JUMP_IMPULSE;
        m_isGrounded = false;
    }

    // Per-axis collision
    // X-axis
    glm::vec3 nextPosX = m_position + glm::vec3(moveDir.x, 0.0f, 0.0f);
    if (!checkCollision(world, getAABB(nextPosX))) {
        m_position.x = nextPosX.x;
    }

    // Y-axis (Gravity/Jump)
    m_isGrounded = false;
    glm::vec3 nextPosY = m_position + glm::vec3(0.0f, m_velocity.y * dt, 0.0f);
    if (!checkCollision(world, getAABB(nextPosY))) {
        m_position.y = nextPosY.y;
    } else {
        // Hit something vertically
        if (m_velocity.y < 0.0f) {
            m_isGrounded = true; // Hit the floor
        }
        m_velocity.y = 0.0f;
    }

    // Z-axis
    glm::vec3 nextPosZ = m_position + glm::vec3(0.0f, 0.0f, moveDir.z);
    if (!checkCollision(world, getAABB(nextPosZ))) {
        m_position.z = nextPosZ.z;
    }
}

void Player::update(float dt, World* world, InputManager* input) {
    // Update camera look
    float dx = input->getMouseDeltaX();
    float dy = input->getMouseDeltaY();
    if (dx != 0 || dy != 0) {
        m_camera.processMouseMovement(dx, dy);
        input->clearMouseDelta();
    }

    handleMovement(dt, world, input);

    // Sync camera position to player position
    m_camera.Position = m_position + glm::vec3(0.0f, m_height * 0.9f, 0.0f);
}
