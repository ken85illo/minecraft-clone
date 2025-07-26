#pragma once

#include "Chunk.hpp"
#include "Player.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <cstdint>

#define WORLD_RADIUS 5
#define PERMUTATION_SIZE 512
#define NUMBER_OF_OCTAVES 4

class World {
public:
    World(Player* player);
    ~World();
    void render();

    Chunk* getChunk(uint16_t x, uint16_t z);

private:
    std::vector<Chunk> m_chunks;
    Texture* m_texture;
    uint8_t m_textureIndex;
    uint16_t m_size;

    Player* m_player;

    void generateHeightMap(float heightMap[CHUNK_SIZE][CHUNK_SIZE], int32_t chunkX, int32_t chunkZ);
    float fractalBrownianMotion(float x, float y);
    float perlin2D(float x, float y);
    float lerp(float t, float a1, float a2);
    float fade(float t);
    glm::vec2 getConstantVec(int32_t val);
    std::vector<int32_t> makePermutation();
    void shufflePermutation(std::vector<int32_t>& permutation);
};
