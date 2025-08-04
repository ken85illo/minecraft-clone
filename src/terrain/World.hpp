#pragma once

#include "Chunk.hpp"
#include "core/player/Player.hpp"
#include "math/PerlinNoise.hpp"
#include "shader/Shader.hpp"
#include "texture/Texture.hpp"
#include <cstdint>
#include <future>
#include <queue>

#define WORLD_RADIUS 12
#define PERMUTATION_SIZE \
    512 // Must be a power of 2 (e.g. 128, 256, 512, 1024...)
#define AMPLITUDE 0.5f
#define FREQUENCY 0.005f
#define NUMBER_OF_OCTAVES 4

class World {
public:
    World(Player* player);
    ~World();
    void render(bool wireFrameMode, Shader* worldShader, Shader* lineShader);

    Chunk* getChunk(uint16_t x, uint16_t z);

private:
    std::vector<Chunk> m_chunks;
    Texture* m_texture;
    uint16_t m_size;
    PerlinNoise m_perlinNoise;

    std::vector<std::thread> m_chunkThreads;
    std::mutex m_chunkMutex;

    void generateHeightMap(std::array<std::array<float, CHUNK_SIZE>, CHUNK_SIZE>& heightMap,
    int32_t chunkX,
    int32_t chunkZ);
};
