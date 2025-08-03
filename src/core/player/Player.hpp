#pragma once

#include "Camera.hpp"
#include "core/Window.hpp"
#include "input/InputHandler.hpp"
#include "shader/Shader.hpp"
#include "terrain/Chunk.hpp"
#include "texture/Texture.hpp"
#include <stack>

#define RANGE_RADIUS 8

struct ChunkCoords {
    Chunk* chunk;
    uint32_t x, y, z;
};

class Player : public Camera {
public:
    Player();

    void moveFront(float deltaTime) override;
    void moveBack(float deltaTime) override;
    void moveRight(float deltaTime) override;
    void moveLeft(float deltaTime) override;
    void moveUp(float deltaTime) override;
    void moveDown(float deltaTime) override;
    void movementInput(Window* window, float deltaTime) override;

    void drawCursor(Shader* shader);
    void placeBlock();
    void destroyBlock();
    void setCurrentChunk(Chunk* chunk);

private:
    Chunk* m_currentChunk;
    Texture* m_texture;
    uint32_t m_VAO, m_VBO, m_EBO;

    float m_chunkFront;
    float m_chunkBack;
    float m_chunkRight;
    float m_chunkLeft;

    ChunkCoords getCoords(glm::vec3 point);
    void updateCurrentChunk();
};
