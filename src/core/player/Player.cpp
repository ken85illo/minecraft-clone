#include "Player.hpp"

Player::Player()
: Camera(0.1f, 500.0f, glm::vec3(0.0f, 0.0f, 0.0f), 5.0f, 0.1f, 60.0f) {

    // Cursor vertices
    float vertices[20] = {
        -0.002f, -0.002f, 0.0f, 0.0f, 0.0f, // bottom-left
        0.002f, -0.002f, 0.0f, 1.0f, 0.0f,  // bottom-right
        0.002f, 0.002f, 0.0f, 1.0f, 1.0f,   // top-right
        -0.002f, 0.002f, 0.0f, 0.0f, 1.0f,  // top-left
    };

    uint32_t indices[6] = {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
    1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_texture = new Texture(GL_TEXTURE_2D, 1);
    m_texture->bind(0);
    m_texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_texture->loadImage("../res/cursor.png");
}

void Player::setCurrentChunk(Chunk* chunk) {
    if(!chunk)
        return;

    static bool spawn = true;
    if(spawn) {
        m_pos = glm::vec3(chunk->getPosition().x + CHUNK_SIZE / 2.0f,
        chunk->getHighestBlock(), chunk->getPosition().z + CHUNK_SIZE / 2.0f);
        spawn = false;
    }

    m_chunkFront = chunk->getPosition().z + CHUNK_SIZE - 1;
    m_chunkBack = chunk->getPosition().z;
    m_chunkRight = chunk->getPosition().x + CHUNK_SIZE - 1;
    m_chunkLeft = chunk->getPosition().x;
    m_currentChunk = chunk;
}

void Player::updateCurrentChunk() {
    if(!m_currentChunk)
        return;

    if(m_pos.z > m_chunkFront) {
        setCurrentChunk(m_currentChunk->getFrontChunk());
        std::println("You moved to front chunk!");
    } else if(m_pos.z < m_chunkBack) {
        setCurrentChunk(m_currentChunk->getBackChunk());
        std::println("You moved to back chunk!");
    }

    if(m_pos.x > m_chunkRight) {
        setCurrentChunk(m_currentChunk->getRightChunk());
        std::println("You moved to right chunk!");
    } else if(m_pos.x < m_chunkLeft) {
        setCurrentChunk(m_currentChunk->getLeftChunk());
        std::println("You moved to left chunk!");
    }
}

void Player::moveFront(float deltaTime) {
    float speed = deltaTime * m_currentSpeed;
    m_pos += m_frontXZ * speed;
    updateCurrentChunk();
}

void Player::moveBack(float deltaTime) {
    float speed = deltaTime * m_currentSpeed;
    m_pos -= m_frontXZ * speed;
    updateCurrentChunk();
}

void Player::moveRight(float deltaTime) {
    float speed = deltaTime * m_currentSpeed;
    m_pos += glm::normalize(glm::cross(m_up, -m_frontXZ)) * speed;
    updateCurrentChunk();
}

void Player::moveLeft(float deltaTime) {
    float speed = deltaTime * m_currentSpeed;
    m_pos -= glm::normalize(glm::cross(m_up, -m_frontXZ)) * speed;
    updateCurrentChunk();
}

void Player::moveUp(float deltaTime) {
    float speed = deltaTime * m_currentSpeed;
    m_pos += m_up * speed;
}

void Player::moveDown(float deltaTime) {
    float speed = deltaTime * m_currentSpeed;
    m_pos -= m_up * speed;
}

void Player::drawCursor(bool wireFrameMode, Shader* shader) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.2f));

    if(wireFrameMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    m_texture->bind(0);
    shader->use();
    shader->setMat4("model", model);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Player::placeBlock() {
    glm::vec3 rayOrigin = m_pos + (m_front * 0.1f);
    glm::vec3 rayDirection = m_front;

    for(float length = 0.0f; length <= floor(RANGE_RADIUS / 2.0f); length += 0.1f) {
        glm::vec3 point = rayOrigin + length * rayDirection;
        auto [chunk, x, y, z] = getCoords(point);

        if(chunk->isAirBlock(x, y, z))
            continue;

        glm::vec3 normals[6] = {
            { 1.5f, 0.0f, 0.0f }, { -0.5f, 0.0f, 0.0f }, // X axis
            { 0.0f, 1.5f, 0.0f }, { 0.0f, -0.5f, 0.0f }, // Y axis
            { 0.0f, 0.0f, 1.5f }, { 0.0f, 0.0f, -0.5f }, // Z axis
        };

        Block::Rect blockRect = chunk->getBlock(x, y, z)->getGlobalRect();
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

        auto [bchunk, bx, by, bz] = getCoords(blockRect.min + normal);
        if(!bchunk->isAirBlock(bx, by, bz))
            break;

        bchunk->updateMesh(Block::OAK_WOOD_BLOCK, bx, by, bz);
        break;
    }
}

void Player::destroyBlock() {
    glm::vec3 rayOrigin = m_pos + (m_front * 0.1f);
    glm::vec3 rayDirection = m_front;

    for(float length = 0.0f; length <= floor(RANGE_RADIUS / 2.0f); length += 0.1f) {
        glm::vec3 point = rayOrigin + length * rayDirection;
        auto [chunk, x, y, z] = getCoords(point);

        if(chunk->isAirBlock(x, y, z))
            continue;

        chunk->updateMesh(Block::AIR, x, y, z);
        break;
    }
}

ChunkCoords Player::getCoords(glm::vec3 point) {
    if(!m_currentChunk || point.y < 0.0f || point.y >= MAX_HEIGHT)
        return ChunkCoords{ nullptr, 0, 0, 0 };

    Chunk* blockChunk = m_currentChunk;
    uint8_t halfChunk = CHUNK_SIZE / 2;
    float dx = point.x - blockChunk->getPosition().x; // leftmost
    float dz = point.z - blockChunk->getPosition().z; // backmost

    if(dx < 0) {
        blockChunk = (blockChunk->getLeftChunk()) ? blockChunk->getLeftChunk() : blockChunk;
        dx += CHUNK_SIZE;
    } else if(dx >= CHUNK_SIZE) {
        blockChunk = (blockChunk->getRightChunk()) ? blockChunk->getRightChunk() : blockChunk;
        dx -= CHUNK_SIZE;
    }

    if(dz < 0) {
        blockChunk = (blockChunk->getBackChunk()) ? blockChunk->getBackChunk() : blockChunk;
        dz += CHUNK_SIZE;
    } else if(dz >= CHUNK_SIZE) {
        blockChunk = (blockChunk->getFrontChunk()) ? blockChunk->getFrontChunk() : blockChunk;
        dz -= CHUNK_SIZE;
    }

    uint16_t nx = floor(dx);
    uint16_t ny = floor(point.y);
    uint16_t nz = floor(dz);

    return ChunkCoords(blockChunk, nx, ny, nz);
}

void Player::movementInput(Window* window, float deltaTime) {
    onCursorMove(InputHandler::getMousePosition().x,
    InputHandler::getMousePosition().y, window->getWidth(), window->getHeight());
    onScroll(InputHandler::getMouseScroll().x, InputHandler::getMouseScroll().y);

    if(InputHandler::isKeyHeld(GLFW_KEY_W))
        moveFront(deltaTime);
    if(InputHandler::isKeyHeld(GLFW_KEY_S))
        moveBack(deltaTime);
    if(InputHandler::isKeyHeld(GLFW_KEY_D))
        moveRight(deltaTime);
    if(InputHandler::isKeyHeld(GLFW_KEY_A))
        moveLeft(deltaTime);
    if(InputHandler::isKeyHeld(GLFW_KEY_SPACE))
        moveUp(deltaTime);
    if(InputHandler::isKeyHeld(GLFW_KEY_LEFT_CONTROL))
        moveDown(deltaTime);
    if(InputHandler::isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
        destroyBlock();
    if(InputHandler::isMousePressed(GLFW_MOUSE_BUTTON_RIGHT))
        placeBlock();
    if(InputHandler::isKeyHeld(GLFW_KEY_LEFT_SHIFT))
        speedUp();
    else
        speedDown();
}
