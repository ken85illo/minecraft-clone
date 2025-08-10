#include "TerrainGenerator.hpp"

void TerrainGenerator::generateHeightMap(
std::array<std::array<float, CHUNK_SIZE>, CHUNK_SIZE>& heightMap,
int32_t chunkX,
int32_t chunkZ) {
    static PerlinNoise m_perlinNoise(FREQUENCY, AMPLITUDE, PERMUTATION_SIZE, NUMBER_OF_OCTAVES);

    for(int32_t x = 0; x < CHUNK_SIZE; x++) {
        for(int32_t z = 0; z < CHUNK_SIZE; z++) {
            float n = m_perlinNoise.fractalBrownianMotion(
            (chunkX * CHUNK_SIZE + x), (chunkZ * CHUNK_SIZE + z));

            n += 1.0f;
            n /= 2.0f;

            heightMap[x][z] = n;
        }
    }
}
