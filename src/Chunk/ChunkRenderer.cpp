#include "ChunkRenderer.hpp"
#include "ChunkMesh.hpp"
#include <algorithm>

ChunkRenderer::ChunkRenderer() {
    glGenVertexArrays(2, m_VAO);
    glGenBuffers(2, m_VBO);
    glGenBuffers(2, m_TBO);
    glGenBuffers(2, m_EBO);
}

ChunkRenderer::~ChunkRenderer() {
    glDeleteBuffers(2, m_VBO);
    glDeleteBuffers(2, m_TBO);
    glDeleteBuffers(2, m_EBO);
    glDeleteVertexArrays(2, m_VAO);
}

void ChunkRenderer::uploadMesh(const MeshData &mesh, MeshType type) {
    size_t index = static_cast<size_t>(type);
    m_indexCount[index] = static_cast<size_t>(mesh.indices.size());

    glBindVertexArray(m_VAO[index]);

    // Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO[index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(uint32_t), mesh.indices.data(), GL_STATIC_DRAW);

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[index]);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(float), mesh.vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Texture coords
    glBindBuffer(GL_ARRAY_BUFFER, m_TBO[index]);
    glBufferData(GL_ARRAY_BUFFER, mesh.texCoords.size() * sizeof(float), mesh.texCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ChunkRenderer::render(MeshType type) {
    size_t index = static_cast<size_t>(type);
    if (index == 1) {
        glDisable(GL_CULL_FACE);
    }

    glBindVertexArray(m_VAO[index]);
    glDrawElements(GL_TRIANGLES, m_indexCount[index], GL_UNSIGNED_INT, 0);

    if (index == 1) {
        glEnable(GL_CULL_FACE);
    }
}
