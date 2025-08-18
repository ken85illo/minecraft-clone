#pragma once

#include "Chunk/Chunk.hpp"
#include "Shader/Shader.hpp"
#include "Texture/Texture.hpp"
#include "Utils/ThreadPool.hpp"
#include <cstdint>
#include <map>

#define WORLD_RADIUS 12

class World {
public:
    World();

    static World* get();

    void render(bool wireFrameMode, Shader* worldShader, Shader* lineShader);
    void sortChunks();
    void sortChunkFaces(int32_t chunkX, int32_t chunkZ, uint8_t radius);

    void generateChunkRight();
    void generateChunkLeft();
    void generateChunkFront();
    void generateChunkBack();

    const int32_t getDiameter() const;
    Chunk* getChunk(int32_t x, int32_t z);

private:
    static std::unique_ptr<World> s_instance;
    static constexpr int32_t m_diameter = WORLD_RADIUS * 2 + 1;
    std::unique_ptr<Texture> m_texture;
    glm::ivec3 m_offset = { 0, 0, 0 };

    std::array<std::array<std::unique_ptr<Chunk>, m_diameter>, m_diameter> m_chunks;
    std::multimap<float, Chunk*> m_sortedChunks;
    ThreadPool m_chunkThreads;

    void initChunk(int32_t ix, int32_t iz, int32_t chunkX, int32_t chunkZ);
    Chunk* generateChunkMeshAsync(int32_t chunkX, int32_t chunkZ);
    void initTexture();
};
