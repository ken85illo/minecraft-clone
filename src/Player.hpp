#pragma once

#include "Camera.hpp"
#include "Chunk.hpp"
#include "Engine.hpp"
#include <stack>

#define RANGE_RADIUS 8

struct ChunkCoords {
    Chunk* chunk;
    uint32_t x, y, z;
};

class Player : public Camera {
public:
    Player();

    void moveFront() override;
    void moveBack() override;
    void moveRight() override;
    void moveLeft() override;
    void moveUp() override;
    void moveDown() override;

    void drawRayLine();
    void placeBlock();
    void destroyBlock();
    void setCurrentChunk(Chunk* chunk);

private:
    Chunk* m_currentChunk;
    uint32_t m_VAO, m_VBO;

    float m_chunkFront;
    float m_chunkBack;
    float m_chunkRight;
    float m_chunkLeft;

    ChunkCoords getCoords(glm::vec3 point);
    void updateCurrentChunk();
};
