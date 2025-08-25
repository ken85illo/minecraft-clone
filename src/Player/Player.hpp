#pragma once

#include "Camera.hpp"
#include "Core/Window.hpp"
#include "Math/RayCast.hpp"
#include "Shader/Shader.hpp"
#include "Texture/Texture.hpp"
#include "World/World.hpp"

#define RANGE_RADIUS 8

class Player : public Camera {
public:
    Player();
    ~Player();

    void init();
    static Player *get();

    bool setCurrentChunk(Chunk *chunk);

    void drawCursor(bool wireFrameMode, Shader *shader);
    void placeBlock();
    void destroyBlock();
    void movementInput(Window *window, float deltaTime);

    const ChunkCoords &getChunkCoords() const;

private:
    static std::unique_ptr<Player> s_instance;
    RayCast m_rayCast;
    World *m_world;

    std::unique_ptr<Texture> m_texture;
    uint32_t m_VAO, m_VBO, m_EBO;
    ChunkCoords m_chunkCoord;

    void uploadCursor();
    void initTexture();
    void updateCurrentChunk();
};
