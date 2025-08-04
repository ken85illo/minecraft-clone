#pragma once

#include <glad/glad.h>

#include "terrain/Treeminator.hpp"
#include "terrain/block/Block.hpp"
#include <cstdint>
#include <glm/glm.hpp>
#include <vector>

#define CHUNK_SIZE 16
#define MAX_HEIGHT 256
#define TERRAIN_HEIGHT 128


class Chunk {
public:
    Chunk(std::array<std::array<float, CHUNK_SIZE>, CHUNK_SIZE>& heightMap, glm::vec3 position);

    void render();
    void generateMesh();
    void updateMesh(Block::Type type, int32_t x, int32_t y, int32_t z);
    void bindVertexArray();
    void deleteVertexArray();
    void spawnTrees();
    bool isAirBlock(int32_t x, int32_t y, int32_t z);

    // Getters
    uint8_t getHighestBlock() const;
    Block* getBlock(int32_t x, int32_t y, int32_t z);
    Chunk* getFrontChunk() const;
    Chunk* getBackChunk() const;
    Chunk* getRightChunk() const;
    Chunk* getLeftChunk() const;
    glm::vec3 getPosition() const;

    // Setters
    void setNeighbours(Chunk* front, Chunk* back, Chunk* right, Chunk* left);
    void setHighestBlock(uint8_t height);

    std::vector<Block> m_blocks;

private:
    uint32_t m_indices[6] = {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    uint32_t m_VAO, m_vertVBO, m_texVBO, m_EBO;
    std::vector<float> m_vertexData;
    std::vector<float> m_textureData;
    std::vector<uint32_t> m_indexData;

    // Neighbour chunks
    Chunk *m_frontChunk, *m_backChunk, *m_rightChunk, *m_leftChunk;
    uint8_t m_highestBlock;
    glm::vec3 m_position;

    // Helper functions
    void fillFaces(int32_t x, int32_t y, int32_t z);
    int32_t getIndex(int32_t x, int32_t y, int32_t z) const;
    void regenerateMesh();
};
