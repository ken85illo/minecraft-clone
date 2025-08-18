#pragma once
#include <glm/glm.hpp>
#include <vector>

class Chunk;
class Player;

struct MeshData {
    std::vector<float> vertices;
    std::vector<float> texCoords;
    std::vector<uint32_t> indices;
};

enum class MeshType : uint8_t {
    OPAQUE = 0,
    TRANSPARENT = 1,
    TOTAL_MESHES = 2,
};

class ChunkMesh {
public:
    static MeshData buildOpaque(Chunk &chunk);
    static MeshData buildTransparent(Chunk &chunk);

private:
    static void fillFaces(Chunk &chunk, int32_t x, int32_t y, int32_t z, MeshData &mesh);
    static bool hasFace(Chunk &chunk, int32_t x, int32_t y, int32_t z);
};
