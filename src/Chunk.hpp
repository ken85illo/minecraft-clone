#pragma once

#include <glad/glad.h>

#include "Atlas.hpp"
#include <cstdint>
#include <glm/glm.hpp>
#include <map>
#include <print>
#include <utility>
#include <vector>

#define CHUNK_SIZE 16
#define MAX_HEIGHT 256

enum BlockType { DIRT_BLOCK = 0, GRASS_BLOCK, STONE_BLOCK, AIR };
enum BlockTexture { DIRT = 0, GRASS_SIDE, GRASS_TOP, STONE };

struct BlockRect {
    glm::vec3 position;
    glm::vec3 min;
    glm::vec3 max;
};

struct Data {
    int32_t index;
    int32_t size;
};

class Chunk {
public:
    Chunk(float heightMap[CHUNK_SIZE][CHUNK_SIZE], glm::vec3 pos);

    std::vector<BlockType> blocks;
    glm::vec3 position;
    int32_t highestBlock;

    // Neighbour chunks
    Chunk *front, *back, *right, *left;

    void render();
    void setNeighbours(Chunk* front, Chunk* back, Chunk* right, Chunk* left);
    void generateMesh();
    void updateMesh(BlockType type, int32_t x, int32_t y, int32_t z);
    void bindVertexArray();
    int32_t getIndex(int32_t x, int32_t y, int32_t z) const;
    BlockType getType(int32_t x, int32_t y, int32_t z) const;
    BlockRect getBlockRect(int32_t x, int32_t y, int32_t z) const;

private:
    float m_vertices[6][12] = {
        // front face
        -0.5f, -0.5f, 0.5f, // bottom-left
        0.5f, -0.5f, 0.5f,  // bottom-right
        0.5f, 0.5f, 0.5f,   // top-right
        -0.5f, 0.5f, 0.5f,  // top-left

        // back face
        0.5f, -0.5f, -0.5f,  // bottom-left
        -0.5f, -0.5f, -0.5f, // bottom-right
        -0.5f, 0.5f, -0.5f,  // top-right
        0.5f, 0.5f, -0.5f,   // top-left

        // top face
        -0.5f, 0.5f, 0.5f,  // bottom-left
        0.5f, 0.5f, 0.5f,   // bottom-right
        0.5f, 0.5f, -0.5f,  // top-right
        -0.5f, 0.5f, -0.5f, // top-left

        // bottom face
        0.5f, -0.5f, 0.5f,   // bottom-left
        -0.5f, -0.5f, 0.5f,  // bottom-right
        -0.5f, -0.5f, -0.5f, // top-right
        0.5f, -0.5f, -0.5f,  // top-left

        // right face
        0.5f, -0.5f, 0.5f,  // bottom-left
        0.5f, -0.5f, -0.5f, // bottom-right
        0.5f, 0.5f, -0.5f,  // top-right
        0.5f, 0.5f, 0.5f,   // top-left

        // left face
        -0.5f, -0.5f, -0.5f, // bottom-left
        -0.5f, -0.5f, 0.5f,  // bottom-right
        -0.5f, 0.5f, 0.5f,   // top-right
        -0.5f, 0.5f, -0.5f,  // top-left
    };

    float m_texCoords[6][8];
    uint16_t m_textures[3][6] = {
        { DIRT, DIRT, DIRT, DIRT, DIRT, DIRT }, // DIRT_BLOCK
        { GRASS_SIDE, GRASS_SIDE, GRASS_TOP, DIRT, GRASS_SIDE, GRASS_SIDE }, // GRASS_BLOCK
        { STONE, STONE, STONE, STONE, STONE, STONE }, // STONE BLOCK
    };

    uint32_t m_indices[6] = {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    uint32_t m_VAO, m_vertVBO, m_texVBO, m_EBO;
    std::vector<float> m_vertexData;
    std::vector<float> m_textureData;
    std::vector<uint32_t> m_indexData;

    std::map<int32_t, std::array<Data, 3>> m_dataMap;

    void fillFaces(int32_t x, int32_t y, int32_t z);
    void refillFaces(int32_t x, int32_t y, int32_t z);
};
