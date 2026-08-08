#include "Player.h"
#include "Config.h"
#include "Raycaster.h"
#include <cmath>

Player::Player(glm::vec3 startPos) : m_velocity(0.0f) {
    m_position = startPos;
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
    // Add small epsilon to prevent snagging on flush wall boundaries
    int minX = std::floor(aabb.min.x + 0.001f);
    int maxX = std::floor(aabb.max.x - 0.001f);
    int minY = std::floor(aabb.min.y + 0.001f);
    int maxY = std::floor(aabb.max.y - 0.001f);
    int minZ = std::floor(aabb.min.z + 0.001f);
    int maxZ = std::floor(aabb.max.z - 0.001f);

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
    float speed = Config::PLAYER_SPEED;
    // GLUT_KEY_SHIFT_L = 112, GLUT_KEY_SHIFT_R = 113
    if (m_inputEnabled && (input->isSpecialKeyPressed(112) || input->isSpecialKeyPressed(113))) {
        speed *= Config::PLAYER_SPRINT_MULTIPLIER;
    }
    float moveSpeed = speed * dt;
    glm::vec3 moveDir(0.0f);

    if (m_inputEnabled) {
        if (input->isKeyPressed('w')) moveDir += m_camera.Front;
        if (input->isKeyPressed('s')) moveDir -= m_camera.Front;
        if (input->isKeyPressed('a')) moveDir -= m_camera.Right;
        if (input->isKeyPressed('d')) moveDir += m_camera.Right;
    }

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

    if (m_isGrounded && m_inputEnabled && input->isKeyPressed(' ')) {
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

    // Out of bounds reset (fell off the world)
    if (m_position.y < -50.0f) {
        // Reset to above the center of the world
        m_position = glm::vec3(
            (Config::WORLD_CHUNKS_X * Config::CHUNK_SIZE) / 2.0f,
            (Config::WORLD_CHUNKS_Y * Config::CHUNK_SIZE) + 20.0f,
            (Config::WORLD_CHUNKS_Z * Config::CHUNK_SIZE) / 2.0f
        );
        m_velocity = glm::vec3(0.0f);
    }
}

void Player::update(float dt, World* world, InputManager* input) {
    // Update camera look
    if (m_inputEnabled) {
        float dx = input->getMouseDeltaX();
        float dy = input->getMouseDeltaY();
        if (dx != 0 || dy != 0) {
            m_camera.processMouseMovement(dx, dy);
            input->clearMouseDelta();
        }

        // --- Block Interaction ---
        // Left click: Break block
        if (input->wasMouseButtonJustPressed(0)) {
            RaycastResult hit = Raycaster::raycast(world, m_camera.Position, m_camera.Front, 8.0f);
            if (hit.hit) {
                world->setBlockAt(hit.hitBlockPos.x, hit.hitBlockPos.y, hit.hitBlockPos.z, BlockType::AIR, false);
            }
        }
        
        // Middle click: Place Crystal
        if (input->wasMouseButtonJustPressed(1)) {
            RaycastResult hit = Raycaster::raycast(world, m_camera.Position, m_camera.Front, 8.0f);
            if (hit.hit) {
                glm::ivec3 placePos = hit.hitBlockPos + hit.hitNormal;
                world->setBlockAt(placePos.x, placePos.y, placePos.z, BlockType::CRYSTAL, true);
            }
        }

        // Right click: Place Metal
        if (input->wasMouseButtonJustPressed(2)) {
            RaycastResult hit = Raycaster::raycast(world, m_camera.Position, m_camera.Front, 8.0f);
            if (hit.hit) {
                glm::ivec3 placePos = hit.hitBlockPos + hit.hitNormal;
                world->setBlockAt(placePos.x, placePos.y, placePos.z, BlockType::METAL, true);
            }
        }
        
        bool interactPressed = input->isKeyPressed('e') || input->isKeyPressed('E');
        if (interactPressed && !m_wasInteractPressed) {
            // Find entities in the world to interact with
            for (const auto& entity : world->getEntities()) {
                float dist = glm::distance(m_position, entity->getPosition());
                if (dist < 10.0f) {
                    entity->onInteract(this);
                    break; // Interact with one entity at a time
                }
            }
        }
        m_wasInteractPressed = interactPressed;
    }

    handleMovement(dt, world, input);

    // Sync camera position to player position
    m_camera.Position = m_position + glm::vec3(0.0f, m_height * 0.9f, 0.0f);
}
