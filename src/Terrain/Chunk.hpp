#pragma once

#include <glad/glad.h>

#include "Terrain/Block/Block.hpp"
#include "Terrain/Treeminator.hpp"
#include <cstdint>
#include <glm/glm.hpp>
#include <thread>
#include <vector>

#define CHUNK_SIZE 16
#define MAX_HEIGHT 256
#define TERRAIN_HEIGHT 128

struct ChunkCoord {
    int32_t chunkX;
    int32_t chunkZ;
};

struct ChunkBounds {
    glm::vec3 min;
    glm::vec3 max;
};

class Player;

class Chunk {
public:
    Chunk(std::array<std::array<float, CHUNK_SIZE>, CHUNK_SIZE>& heightMap, glm::vec3 position);

    void render(uint8_t index);
    void generateMesh();
    void sortTransparent();

    void updateMesh(Block::Type type, int32_t x, int32_t y, int32_t z);
    void bindVertexArray(uint8_t index);
    void generateTransparent();
    void deleteVertexArray();
    void spawnTrees();
    bool isAirBlock(int32_t x, int32_t y, int32_t z);

    static void loadPlayer(Player* player);

    // Getters
    uint8_t getHighestBlock() const;
    Block* getBlock(int32_t x, int32_t y, int32_t z);
    Chunk* getFrontChunk() const;
    Chunk* getBackChunk() const;
    Chunk* getRightChunk() const;
    Chunk* getLeftChunk() const;
    glm::vec3 getPosition() const;
    const ChunkBounds& getBounds() const;

    // Setters
    void setNeighbours(Chunk* front, Chunk* back, Chunk* right, Chunk* left);
    void setHighestBlock(uint8_t height);


private:
    uint32_t m_indices[2][6] = {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle

        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    uint32_t m_VAO[2], m_vertVBO[2], m_texVBO[2], m_EBO[2];
    std::vector<float> m_vertexData[2];
    std::vector<float> m_textureData[2];
    std::vector<uint32_t> m_indexData[2];
    uint32_t m_indexCount[2];
    Block m_blocks[CHUNK_SIZE][MAX_HEIGHT][CHUNK_SIZE];

    // Neighbour chunks
    Chunk *m_frontChunk, *m_backChunk, *m_rightChunk, *m_leftChunk;

    uint8_t m_highestBlock;
    std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> m_transparentBlocks;
    glm::vec3 m_position;
    ChunkBounds m_chunkBounds;

    static Player* s_player;

    // Helper functions
    void fillFaces(int32_t x, int32_t y, int32_t z, uint8_t index);
    void regenerateMesh();
};
