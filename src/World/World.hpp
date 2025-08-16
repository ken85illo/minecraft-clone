#pragma once

#include "Chunk/Chunk.hpp"
#include "Shader/Shader.hpp"
#include "Texture/Texture.hpp"
#include "Utils/ThreadPool.hpp"
#include <cstdint>
#include <map>

#define WORLD_RADIUS 20

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
    const int32_t getIndex(int32_t x, int32_t z) const;
    Chunk* getChunk(int32_t x, int32_t z);

private:
    static std::unique_ptr<World> s_instance;
    std::unique_ptr<Texture> m_texture;
    int32_t m_diameter;
    glm::ivec3 m_offset = { 0, 0, 0 };

    std::vector<std::unique_ptr<Chunk>> m_chunks;
    std::multimap<float, Chunk*> m_sortedChunks;
    ThreadPool m_chunkThreads;

    void initChunk(int32_t chunkX, int32_t chunkZ, std::vector<std::unique_ptr<Chunk>>::iterator it);
    Chunk* generateChunkMeshAsync(int32_t chunkX, int32_t chunkZ);
    void initTexture();
};
