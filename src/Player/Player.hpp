#pragma once

#include <glad/glad.h>

#include "Camera.hpp"
#include "Core/Window.hpp"
#include "Input/InputHandler.hpp"
#include "Shader/Shader.hpp"
#include "Texture/Texture.hpp"
#include "World/World.hpp"
#include <stack>

#define RANGE_RADIUS 8

struct RayCoords {
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

    void drawCursor(bool wireFrameMode, Shader* shader);
    void placeBlock();
    void destroyBlock();
    void setSpawn(World* world);
    bool setCurrentChunk(Chunk* chunk);

    const ChunkCoord& getChunkCoord() const;

private:
    Chunk* m_currentChunk;
    ChunkCoord m_chunkCoord = { 0, 0 };
    std::unique_ptr<Texture> m_texture;
    uint32_t m_VAO, m_VBO, m_EBO;

    RayCoords getCoords(glm::vec3 point);
    void updateCurrentChunk();
};
