#pragma once

#include "Block/Block.hpp"
#include "ChunkMesh.hpp"
#include <cstdint>

struct MeshData;
class Chunk;

class ChunkManager {
  public:
    static void updateMesh(Chunk &chunk, const MeshData &data, MeshType meshType);
    static void uploadMesh(Chunk &chunk, MeshType meshType);
    static void updateBlock(Chunk &chunk, int32_t x, int32_t y, int32_t z, BlockType blockType);
    static void render(Chunk &chunk, MeshType type);

  private:
    static void buildMesh(Chunk &chunk);
    static void updateNeighbour(Chunk *neighbourChunk, bool condition, int32_t x, int32_t y, int32_t z);
};
