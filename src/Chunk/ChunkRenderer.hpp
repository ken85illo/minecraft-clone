#pragma once

struct MeshData;
enum class MeshType : uint8_t;

class ChunkRenderer {
public:
    ChunkRenderer();
    ~ChunkRenderer();

    void init();
    void uploadMesh(const MeshData &mesh, MeshType type);
    void render(MeshType type);

private:
    uint32_t m_VAO[2];
    uint32_t m_VBO[2];
    uint32_t m_TBO[2];
    uint32_t m_EBO[2];
    size_t m_indexCount[2];
    bool m_initialized = false;

    friend class ChunkManager;
};
