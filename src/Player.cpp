#include "Player.hpp"

Player::Player()
: Camera(0.1f, 300.0f, glm::vec3(0.0f, 0.0f, 0.0f), 5.0f, 0.1f, 60.0f),
  m_currentChunk(nullptr),
  m_chunkFront(0.0f),
  m_chunkBack(0.0f),
  m_chunkLeft(0.0f),
  m_chunkRight(0.0f) {

    // draw a ray
    float vertices[2][3]{
        { 0.0f, 0.0f, 0.0f }, //
        { 1.0f, 0.0f, 0.0f }, //
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Player::setCurrentChunk(Chunk* chunk) {
    if(!chunk)
        return;

    static bool spawn = true;
    if(spawn) {
        m_pos =
        glm::vec3(chunk->position.x, chunk->highestBlock, chunk->position.z);
        spawn = false;
    }

    float halfChunk = CHUNK_SIZE / 2.0f;
    m_chunkFront = chunk->position.z + halfChunk;
    m_chunkBack = chunk->position.z - halfChunk;
    m_chunkRight = chunk->position.x + halfChunk;
    m_chunkLeft = chunk->position.x - halfChunk;
    m_currentChunk = chunk;
}

void Player::updateCurrentChunk() {
    if(!m_currentChunk)
        return;

    if(m_pos.z > m_chunkFront) {
        setCurrentChunk(m_currentChunk->front);
        std::println("You moved to front chunk!");
    } else if(m_pos.z < m_chunkBack) {
        setCurrentChunk(m_currentChunk->back);
        std::println("You moved to back chunk!");
    }

    if(m_pos.x > m_chunkRight) {
        setCurrentChunk(m_currentChunk->right);
        std::println("You moved to right chunk!");
    } else if(m_pos.x < m_chunkLeft) {
        setCurrentChunk(m_currentChunk->left);
        std::println("You moved to left chunk!");
    }
}

void Player::moveFront() {
    float speed = Engine::deltaTime * m_currentSpeed;
    m_pos += m_frontXZ * speed;
    updateCurrentChunk();
}

void Player::moveBack() {
    float speed = Engine::deltaTime * m_currentSpeed;
    m_pos -= m_frontXZ * speed;
    updateCurrentChunk();
}

void Player::moveRight() {
    float speed = Engine::deltaTime * m_currentSpeed;
    m_pos += glm::normalize(glm::cross(m_frontXZ, m_up)) * speed;
    updateCurrentChunk();
}

void Player::moveLeft() {
    float speed = Engine::deltaTime * m_currentSpeed;
    m_pos -= glm::normalize(glm::cross(m_frontXZ, m_up)) * speed;
    updateCurrentChunk();
}

void Player::moveUp() {
    float speed = Engine::deltaTime * m_currentSpeed;
    m_pos += m_up * speed;
}

void Player::moveDown() {
    float speed = Engine::deltaTime * m_currentSpeed;
    m_pos -= m_up * speed;
}


void Player::drawRayLine() {
    glm::vec3 rayOrigin = m_pos + (m_front * 0.1f);
    glm::vec3 rayDirection = m_front;

    for(float length = 0.0f; length <= floor(RANGE_RADIUS / 2.0f); length += 0.1f) {
        glm::vec3 point = rayOrigin + length * rayDirection;
        auto [chunk, x, y, z] = getCoords(point);

        if(!chunk)
            continue;

        if(chunk->getType(x, y, z) == AIR) {
            length += 1.0f;
            continue;
        }

        glm::vec3 dir = glm::normalize(-m_front);
        glm::vec3 right = glm::normalize(glm::cross(m_up, dir));
        glm::vec3 up = glm::cross(dir, right);

        glm::mat4 rotation = glm::mat4(1.0f);
        rotation[0] = glm::vec4(right.x, up.x, dir.x, 0.0f);
        rotation[1] = glm::vec4(right.y, up.y, dir.y, 0.0f);
        rotation[2] = glm::vec4(right.z, up.z, dir.z, 0.0f);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, rayOrigin);
        model *= rotation;
        model = glm::scale(model, glm::vec3(1.0f, length, 1.0f));

        Engine::lineShader->use();
        Engine::lineShader->setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
        Engine::lineShader->setMat4("model", model);

        glLineWidth(10.0f);
        glBindVertexArray(m_VAO);
        glDrawArrays(GL_LINES, 0, 2);
        glLineWidth(1.0f);
        break;
    }
}

void Player::placeBlock() {
    glm::vec3 rayOrigin = m_pos + (m_front * 0.1f);
    glm::vec3 rayDirection = m_front;

    for(float length = 0.0f; length <= floor(RANGE_RADIUS / 2.0f); length += 0.1f) {
        glm::vec3 point = rayOrigin + length * rayDirection;
        auto [chunk, x, y, z] = getCoords(point);

        if(!chunk)
            continue;

        if(chunk->getType(x, y, z) == AIR) {
            length += 0.5f;
            continue;
        }

        glm::vec3 normals[6] = {
            { 1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, // X axis
            { 0.0f, 1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, // Y axis
            { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, -1.0f }, // Z axis
        };

        BlockRect blockRect = chunk->getBlockRect(x, y, z);
        float minDist = 0.0f;
        glm::vec3 normal = glm::vec3(0.0f);

        for(uint8_t i = 0; i < 3; i++) {
            if(rayDirection[i] == 0.0f)
                continue;

            // Transpose version of blockRect.side = rayOrigin + length * rayDirection;
            float dist1 = (blockRect.min[i] - rayOrigin[i]) / rayDirection[i];
            float dist2 = (blockRect.max[i] - rayOrigin[i]) / rayDirection[i];

            // If dist2 is greater than dist1 then its on the opposite side
            int8_t face = i * 2 + (dist1 < dist2);

            if(dist1 > dist2)
                std::swap(dist1, dist2);

            if(minDist < dist1) {
                minDist = dist1;
                normal = normals[face];
            }
        }

        auto [bchunk, bx, by, bz] = getCoords(blockRect.position + normal);
        if(bchunk->getType(bx, by, bz) != AIR)
            break;

        bchunk->updateMesh(STONE_BLOCK, bx, by, bz);
        break;
    }
}

void Player::destroyBlock() {
    glm::vec3 rayOrigin = m_pos + (m_front * 0.1f);
    glm::vec3 rayDirection = m_front;

    for(float length = 0.0f; length <= floor(RANGE_RADIUS / 2.0f); length += 0.1f) {
        glm::vec3 point = rayOrigin + length * rayDirection;
        auto [chunk, x, y, z] = getCoords(point);

        if(!chunk)
            continue;

        if(chunk->getType(x, y, z) == AIR) {
            length += 0.5f;
            continue;
        }

        chunk->updateMesh(AIR, x, y, z);
        break;
    }
}

ChunkCoords Player::getCoords(glm::vec3 point) {
    if(!m_currentChunk || point.y < 0.0f || point.y >= MAX_HEIGHT)
        return ChunkCoords{ nullptr, 0, 0, 0 };

    Chunk* blockChunk = m_currentChunk;
    float halfChunk = CHUNK_SIZE / 2.0f;
    float dx = point.x + halfChunk - blockChunk->position.x; // leftmost
    float dz = point.z + halfChunk - blockChunk->position.z; // backmost

    if(dx < 0) {
        blockChunk = (blockChunk->left) ? blockChunk->left : blockChunk;
        dx += CHUNK_SIZE;
    } else if(dx >= CHUNK_SIZE) {
        blockChunk = (blockChunk->right) ? blockChunk->right : blockChunk;
        dx -= CHUNK_SIZE;
    }

    if(dz < 0) {
        blockChunk = (blockChunk->back) ? blockChunk->back : blockChunk;
        dz += CHUNK_SIZE;
    } else if(dz >= CHUNK_SIZE) {
        blockChunk = (blockChunk->front) ? blockChunk->front : blockChunk;
        dz -= CHUNK_SIZE;
    }

    uint16_t nx = floor(dx);
    uint16_t ny = floor(point.y);
    uint16_t nz = floor(dz);

    return ChunkCoords(blockChunk, nx, ny, nz);
}
