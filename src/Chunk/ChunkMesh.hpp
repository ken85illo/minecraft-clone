#pragma once
#include <algorithm>
#include <glm/glm.hpp>
#include <tuple>
#include <vector>

class Chunk;
class Player;

struct MeshData {
    std::vector<float> vertices;
    std::vector<float> texCoords;
    std::vector<uint32_t> indices;
};

class ChunkMesh {
public:
    static MeshData buildOpaque(Chunk& chunk);
    static MeshData buildTransparent(Chunk& chunk);

private:
    static void fillFaces(Chunk& chunk, int32_t x, int32_t y, int32_t z, MeshData& mesh);
};
