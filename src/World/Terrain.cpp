#include "Terrain.hpp"
#include "Utils/Serializer.hpp"

std::unique_ptr<PerlinNoise> Terrain::s_perlinNoise;

void Terrain::init() {
    if (Serializer::fileExists("noise")) {
        s_perlinNoise.reset(Serializer::read<PerlinNoise>("noise"));
    }
    else {
        s_perlinNoise = std::make_unique<PerlinNoise>(FREQUENCY, AMPLITUDE, NUMBER_OF_OCTAVES);
        Serializer::write(*s_perlinNoise, "noise");
    }
}

void Terrain::generateHeightMap(
    std::array<std::array<float, CHUNK_SIZE>, CHUNK_SIZE> &heightMap, int32_t chunkX, int32_t chunkZ
) {

    for (int32_t x = 0; x < CHUNK_SIZE; x++) {
        for (int32_t z = 0; z < CHUNK_SIZE; z++) {
            float n = s_perlinNoise->fractalBrownianMotion((chunkX * CHUNK_SIZE + x), (chunkZ * CHUNK_SIZE + z));

            n = (n + 1.0f) * 0.5f;

            heightMap[x][z] = n;
        }
    }
}
