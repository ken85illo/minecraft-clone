#pragma once

#include "Chunk/ChunkContext.hpp"
#include "Math/PerlinNoise.hpp"
#include <array>

#define PERMUTATION_SIZE 512 // Must be a power of 2 (e.g. 128, 256, 512, 1024...)
#define AMPLITUDE 0.5f
#define FREQUENCY 0.005f
#define NUMBER_OF_OCTAVES 5

class Terrain {
public:
    static void generateHeightMap(std::array<std::array<float, CHUNK_SIZE>, CHUNK_SIZE>& heightMap, int32_t chunkX, int32_t chunkZ);
};
