#pragma once

#include <glad/glad.h>

#include "Camera.hpp"
#include "Core/Window.hpp"
#include "Input/InputHandler.hpp"
#include "Math/RayCast.hpp"
#include "Shader/Shader.hpp"
#include "Texture/Texture.hpp"
#include "World/World.hpp"
#include <stack>

#define RANGE_RADIUS 8

class Player : public Camera {
public:
    Player();

    void uploadCursor();
    void initTexture();

    void setSpawn(World* world);
    bool setCurrentChunk(Chunk* chunk);

    void drawCursor(bool wireFrameMode, Shader* shader);
    void placeBlock();
    void destroyBlock();

    Chunk* getCurrentChunk() const;
    const ChunkCoord& getChunkCoord() const;

    void movementInput(Window* window, float deltaTime);

private:
    Chunk* m_currentChunk;
    ChunkCoord m_chunkCoord = { 0, 0 };
    std::unique_ptr<Texture> m_texture;
    uint32_t m_VAO, m_VBO, m_EBO;

    RayCast m_rayCast;

    void updateCurrentChunk();
};
