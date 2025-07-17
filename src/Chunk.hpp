#pragma once

#include <glad/glad.h>

#include "Atlas.hpp"
#include <cstdint>
#include <glm/glm.hpp>
#include <print>
#include <vector>

enum BlockType { DIRT_BLOCK = 0, GRASS_BLOCK, SOLID, AIR };
enum BlockTexture { DIRT = 0, GRASS_SIDE, GRASS_TOP };

class Chunk {
public:
    Chunk(int16_t chunkSize, int16_t height, int16_t maxHeight, glm::vec3 pos);

    std::vector<BlockType> blocks;
    int16_t chunkSize;
    uint16_t height;
    uint16_t maxHeight;

    void render();
    glm::vec3 getPosition() const;
    void generateMesh(Chunk* front, Chunk* back, Chunk* right, Chunk* left);

private:
    uint32_t m_VAO, m_vertVBO, m_texVBO, m_EBO;
    std::vector<glm::vec3> vertexData;
    std::vector<glm::vec2> textureData;
    std::vector<uint32_t> indexData;
    uint8_t m_textureIndex;
    glm::vec3 m_position;

    BlockType getType(int16_t x,
    int16_t y,
    int16_t z,
    Chunk* front = nullptr,
    Chunk* back = nullptr,
    Chunk* right = nullptr,
    Chunk* left = nullptr) const;

    uint16_t getIndex(uint16_t x, uint16_t y, uint16_t z) const;
};
