#pragma once

#include "Camera.hpp"
#include "shader/Shader.hpp"
#include "terrain/Chunk.hpp"
#include <stack>

#define RANGE_RADIUS 8

struct ChunkCoords {
    Chunk* chunk;
    uint32_t x, y, z;
};

class Player : public Camera {
public:
    Player(Shader* lineShader);

    void moveFront(float deltaTime) override;
    void moveBack(float deltaTime) override;
    void moveRight(float deltaTime) override;
    void moveLeft(float deltaTime) override;
    void moveUp(float deltaTime) override;
    void moveDown(float deltaTime) override;

    void drawRayLine();
    void placeBlock();
    void destroyBlock();
    void setCurrentChunk(Chunk* chunk);

private:
    Chunk* m_currentChunk;
    Shader* m_lineShader;
    uint32_t m_VAO, m_VBO;

    float m_chunkFront;
    float m_chunkBack;
    float m_chunkRight;
    float m_chunkLeft;

    ChunkCoords getCoords(glm::vec3 point);
    void updateCurrentChunk();
};
