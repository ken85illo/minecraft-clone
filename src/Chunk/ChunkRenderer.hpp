#pragma once

#include <cstddef>
#include <glad/glad.h>

struct MeshData;

class ChunkRenderer {
public:
    ChunkRenderer();

    void uploadMesh(const MeshData& mesh, uint8_t index);
    void render(uint8_t index);

private:
    uint32_t m_VAO[2];
    uint32_t m_VBO[2];
    uint32_t m_TBO[2];
    uint32_t m_EBO[2];
    size_t m_indexCount[2];

    friend class ChunkManager;
};
