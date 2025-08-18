#pragma once

#include "Block/Block.hpp"
#include "Chunk/ChunkRenderer.hpp"
#include "ChunkContext.hpp"
#include "ChunkManager.hpp"
#include "ChunkMesh.hpp"
#include <array>
#include <glm/glm.hpp>
#include <memory>

class Player;
class Treeminator;

class Chunk {
  public:
    Chunk(const std::array<std::array<float, CHUNK_SIZE>, CHUNK_SIZE> &heightMap, glm::vec3 position);

    void setNeighbours(Chunk *front, Chunk *back, Chunk *right, Chunk *left);

    BlockType *getBlockType(int32_t x, int32_t y, int32_t z);
    const glm::vec3 getBlockLocalPos(int32_t x, int32_t y, int32_t z) const;
    const glm::vec3 getBlockGlobalPos(int32_t x, int32_t y, int32_t z) const;

    void setHighestBlock(uint8_t height);
    uint8_t getHighestBlock() const;

    glm::vec3 getPosition() const;
    const ChunkBounds &getBounds() const;

    Chunk *getFrontChunk() const;
    Chunk *getBackChunk() const;
    Chunk *getRightChunk() const;
    Chunk *getLeftChunk() const;

  private:
    glm::vec3 m_position;
    ChunkBounds m_chunkBounds;

    BlockType m_blocks[CHUNK_SIZE][MAX_HEIGHT][CHUNK_SIZE];
    uint8_t m_highestBlock = 0;

    Chunk *m_frontChunk = nullptr;
    Chunk *m_backChunk = nullptr;
    Chunk *m_rightChunk = nullptr;
    Chunk *m_leftChunk = nullptr;

    MeshData m_meshData[2];
    std::unique_ptr<ChunkRenderer> m_renderer;

    friend class ChunkMesh;
    friend class ChunkManager;
    friend class Treeminator;
};
