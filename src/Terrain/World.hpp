#pragma once

#include "Chunk.hpp"
#include "Core/Player/Player.hpp"
#include "Shader/Shader.hpp"
#include "Texture/Texture.hpp"
#include "Utils/Math/PerlinNoise.hpp"
#include <cstdint>
#include <future>
#include <map>
#include <queue>

#define WORLD_RADIUS 12
#define PERMUTATION_SIZE \
    512 // Must be a power of 2 (e.g. 128, 256, 512, 1024...)
#define AMPLITUDE 0.5f
#define FREQUENCY 0.005f
#define NUMBER_OF_OCTAVES 5

class World {
public:
    World(Player* player);

    ~World();
    void render(bool wireFrameMode, Shader* worldShader, Shader* lineShader);

    const int32_t getDiameter() const;
    const int32_t getIndex(int32_t x, int32_t z) const;
    Chunk* getChunk(int32_t x, int32_t z);

private:
    std::unique_ptr<Texture> m_texture;
    int32_t m_diameter;
    PerlinNoise m_perlinNoise;

    std::vector<Chunk> m_chunks;
    std::vector<std::future<void>> m_chunkThreads;
    Player* m_player;

    void generateHeightMap(std::array<std::array<float, CHUNK_SIZE>, CHUNK_SIZE>& heightMap,
    int32_t chunkX,
    int32_t chunkZ);

    void drawChunk(int32_t index, uint8_t renderIndex, Shader* shader);
};
