#pragma once

#include <cstdint>

struct MeshData;
class Chunk;
class BlockType;

class ChunkManager {
public:
    static void updateMesh(Chunk& chunk, const MeshData& data, uint8_t index);
    static void uploadMesh(Chunk& chunk, uint8_t index);
    static void updateBlock(Chunk& chunk, int32_t x, int32_t y, int32_t z, int8_t type);
    static void buildMesh(Chunk& chunk, const MeshData& data, uint8_t index);
    static void deleteMesh(Chunk& chunk);
    static void render(Chunk& chunk, uint8_t index);

private:
    static void
    updateNeighbour(Chunk* neighbourChunk, bool condition, int32_t x, int32_t y, int32_t z);
};
