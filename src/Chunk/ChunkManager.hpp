#pragma once

#include "Block/Block.hpp"
#include "ChunkMesh.hpp"

struct MeshData;
class Chunk;

class ChunkManager {
public:
    static void updateMesh(Chunk &chunk, const MeshData &data, MeshType meshType);
    static void uploadMesh(Chunk &chunk, MeshType meshType);
    static void updateBlock(Chunk &chunk, int32_t x, int32_t y, int32_t z, BlockType blockType);
    static void render(Chunk &chunk, MeshType type);

    static void serialize(Chunk &chunk, int32_t chunkX, int32_t chunkZ);
    static Chunk deserialize(int32_t chunkX, int32_t chunkZ);
    static bool binaryExists(int32_t chunkX, int32_t chunkZ);

private:
    static void buildMesh(Chunk &chunk);
    static void updateNeighbour(Chunk *neighbourChunk, bool condition, int32_t x, int32_t y, int32_t z);
    static const std::string getBinaryName(int32_t chunkX, int32_t chunkZ);
};
