#pragma once

#include "Chunk/Chunk.hpp"
#include "Shader/Shader.hpp"
#include "Texture/Texture.hpp"
#include "Utils/HashFunctor.hpp"
#include "Utils/ThreadPool.hpp"
#include "World/LightOrigin.hpp"

#define WORLD_RADIUS 20

class World {
public:
    World();

    static World *get();

    void render(float &lightValue, bool wireFrameMode, Shader *worldShader, Shader *lineShader, Shader *lightShader);
    void update(const float deltaTime);
    void sortChunks();
    void sortChunkFaces(int32_t chunkX, int32_t chunkZ, uint8_t radius);

    void generateChunkRight();
    void generateChunkLeft();
    void generateChunkFront();
    void generateChunkBack();

    void saveChunk(int32_t indexX, int32_t indexZ);

    const int32_t getDiameter() const;
    Chunk *getChunk(int32_t x, int32_t z);
    const int32_t getPositionIndex(int32_t index);

private:
    static constexpr int32_t m_lightHeight = (WORLD_RADIUS + 2) * CHUNK_SIZE;
    static std::unique_ptr<World> s_instance;
    static constexpr int32_t m_diameter = WORLD_RADIUS * 2 + 1;
    std::unique_ptr<Texture> m_texture;
    ChunkCoords m_offset = { 0, 0 };

    LightOrigin m_lightOrigin;

    std::array<std::array<std::unique_ptr<Chunk>, m_diameter>, m_diameter> m_chunks;
    std::multimap<float, Chunk *> m_sortedChunks;

    ThreadPool m_chunkThreads;

    std::mutex m_initMutex;
    std::condition_variable m_initCV;

    std::unordered_map<ChunkCoords, bool, HashFunctor> m_waitingChunks;
    std::mutex m_waitingMutex;
    std::condition_variable m_waitingCV;

    void initChunk(int32_t indexX, int32_t indexZ);
    void generateChunkMeshAsync(int32_t indexX, int32_t indexZ);
    void uploadChunk(int32_t indexX, int32_t indexZ);
    void initTexture();
};
