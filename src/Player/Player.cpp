#include "Player.hpp"
#include "Utils/InputHandler.hpp"

std::unique_ptr<Player> Player::s_instance = nullptr;

Player::Player()
: Camera(0.1f, (WORLD_RADIUS * 2 + 1) * CHUNK_SIZE, glm::vec3(0.0f, 0.0f, 0.0f), 7.0f, 0.1f, 60.0f),
  m_rayCast(RANGE_RADIUS) {
    uploadCursor();
    initTexture();
}

void Player::init() {
    m_world = World::get();
    auto diameter = m_world->getDiameter();
    auto chunk = m_world->getChunk(diameter / 2, diameter / 2);

    m_pos = glm::vec3(
        chunk->getPosition().x + CHUNK_SIZE / 2.0f, chunk->getHighestBlock(), chunk->getPosition().z + CHUNK_SIZE / 2.0f
    );
    m_chunkCoord = ChunkCoords(diameter / 2, diameter / 2);
}

Player *Player::get() {
    if (!s_instance) {
        s_instance = std::make_unique<Player>();
    }

    return s_instance.get();
}

void Player::uploadCursor() {
    // Cursor vertices
    float vertices[20] = {
        -0.002f, -0.002f, 0.0f, 0.0f, 0.0f, // bottom-left
        0.002f,  -0.002f, 0.0f, 1.0f, 0.0f, // bottom-right
        0.002f,  0.002f,  0.0f, 1.0f, 1.0f, // top-right
        -0.002f, 0.002f,  0.0f, 0.0f, 1.0f, // top-left
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Player::initTexture() {
    m_texture = std::make_unique<Texture>(GL_TEXTURE_2D, 1);
    m_texture->bind(0);
    m_texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    m_texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_texture->loadImage("res/cursor.png");
}

const ChunkCoords &Player::getChunkCoords() const {
    return m_chunkCoord;
}

void Player::updateCurrentChunk() {
    const ChunkBounds &bounds = m_world->getChunk(m_chunkCoord.x, m_chunkCoord.z)->getBounds();

    if (m_pos.z > bounds.max.z) {
        std::println("You moved to front chunk!");
        m_world->generateChunkFront();
    }

    if (m_pos.z < bounds.min.z) {
        std::println("You moved to back chunk!");
        m_world->generateChunkBack();
    }

    if (m_pos.x > bounds.max.x) {
        std::println("You moved to right chunk!");
        m_world->generateChunkRight();
    }

    if (m_pos.x < bounds.min.x) {
        std::println("You moved to left chunk!");
        m_world->generateChunkLeft();
    }

    static glm::vec3 playerPos = m_pos;
    float distance = glm::length(m_pos - playerPos);

    // Only sort transparent faces when the player moved 1 block from previous position
    if (distance >= 1.0f) {
        float x = std::copysign(std::round(std::fabs(m_frontXZ.x)), m_frontXZ.x) * 2;
        float z = std::copysign(std::round(std::fabs(m_frontXZ.z)), m_frontXZ.z) * 2;

        m_world->sortChunkFaces(m_chunkCoord.x + x, m_chunkCoord.z + z, 1);
        m_world->sortChunkFaces(m_chunkCoord.x, m_chunkCoord.z, 0);
        playerPos = m_pos;
    }
}

void Player::drawCursor(bool wireFrameMode, Shader *shader) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.2f));

    if (wireFrameMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    m_texture->bind(0);
    shader->use();
    shader->setMat4("model", model);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    if (wireFrameMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}

void Player::placeBlock() {
    auto [chunk, x, y, z] = m_rayCast.sendRay();

    if (!chunk) {
        return;
    }

    glm::vec3 rayOrigin = m_rayCast.getRayOrigin();
    glm::vec3 rayDirection = m_rayCast.getRayDirection();

    static const glm::vec3 normals[6] = {
        {  1.5f,  0.0f,  0.0f },
        { -0.5f,  0.0f,  0.0f }, // X axis
        {  0.0f,  1.5f,  0.0f },
        {  0.0f, -0.5f,  0.0f }, // Y axis
        {  0.0f,  0.0f,  1.5f },
        {  0.0f,  0.0f, -0.5f }, // Z axis
    };

    BlockRect blockRect = Block::getGlobalRect(*chunk, x, y, z);
    float minDist = 0.0f;
    glm::vec3 normal = glm::vec3(0.0f);

    for (uint8_t i = 0; i < 3; i++) {
        if (rayDirection[i] == 0.0f) {
            continue;
        }

        // Transpose version of blockRect.side = rayOrigin + length * rayDirection;
        float dist1 = (blockRect.min[i] - rayOrigin[i]) / rayDirection[i];
        float dist2 = (blockRect.max[i] - rayOrigin[i]) / rayDirection[i];

        // If dist2 is greater than dist1 then its on the opposite side
        int8_t face = i * 2 + (dist1 < dist2);

        if (dist1 > dist2) {
            std::swap(dist1, dist2);
        }

        if (minDist < dist1) {
            minDist = dist1;
            normal = normals[face];
        }
    }

    auto [bchunk, bx, by, bz] = m_rayCast.getCoordsAtPoint(blockRect.min + normal);
    BlockType *placeBlock = bchunk->getBlockType(bx, by, bz);

    if (placeBlock && *placeBlock != BlockType::AIR) {
        return;
    }

    ChunkManager::updateBlock(*bchunk, bx, by, bz, BlockType::STONE);
}

void Player::destroyBlock() {
    auto [chunk, x, y, z] = m_rayCast.sendRay();

    if (!chunk) {
        return;
    }

    ChunkManager::updateBlock(*chunk, x, y, z, BlockType::AIR);
}

void Player::movementInput(Window *window, float deltaTime) {
    onCursorMove(
        InputHandler::getMousePosition().x, InputHandler::getMousePosition().y, window->getWidth(), window->getHeight()
    );
    onScroll(InputHandler::getMouseScroll().x, InputHandler::getMouseScroll().y);

    static glm::vec3 prevPos = m_pos;

    if (InputHandler::isKeyHeld(GLFW_KEY_W)) {
        moveFront(deltaTime);
    }

    if (InputHandler::isKeyHeld(GLFW_KEY_S)) {
        moveBack(deltaTime);
    }

    if (InputHandler::isKeyHeld(GLFW_KEY_D)) {
        moveRight(deltaTime);
    }

    if (InputHandler::isKeyHeld(GLFW_KEY_A)) {
        moveLeft(deltaTime);
    }

    if (prevPos != m_pos) {
        updateCurrentChunk();
        prevPos = m_pos;
    }

    if (InputHandler::isKeyHeld(GLFW_KEY_SPACE)) {
        moveUp(deltaTime);
    }

    if (InputHandler::isKeyHeld(GLFW_KEY_LEFT_CONTROL)) {
        moveDown(deltaTime);
    }

    if (InputHandler::isMousePressed(GLFW_MOUSE_BUTTON_LEFT)) {
        destroyBlock();
    }

    if (InputHandler::isMousePressed(GLFW_MOUSE_BUTTON_RIGHT)) {
        placeBlock();
    }

    if (InputHandler::isKeyHeld(GLFW_KEY_LEFT_SHIFT)) {
        speedUp();
    }
    else {
        speedDown();
    }
}
