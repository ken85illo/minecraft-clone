#pragma once

#include "Chunk/ChunkContext.hpp"
#include "Math/PerlinNoise.hpp"

#define AMPLITUDE 0.75f
#define FREQUENCY 0.0025f
#define NUMBER_OF_OCTAVES 6

class Terrain {
public:
    static void init();

    static void
    generateHeightMap(std::array<std::array<float, CHUNK_SIZE>, CHUNK_SIZE> &heightMap, int32_t chunkX, int32_t chunkZ);

private:
    static std::unique_ptr<PerlinNoise> s_perlinNoise;
};
