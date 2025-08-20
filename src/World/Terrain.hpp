#pragma once

#include "Chunk/ChunkContext.hpp"
#include "Math/PerlinNoise.hpp"
#include <array>

#define AMPLITUDE 0.5f
#define FREQUENCY 0.005f
#define NUMBER_OF_OCTAVES 5

class Terrain {
public:
    static void init();

    static void
    generateHeightMap(std::array<std::array<float, CHUNK_SIZE>, CHUNK_SIZE> &heightMap, int32_t chunkX, int32_t chunkZ);

private:
    static PerlinNoise s_perlinNoise;
};
