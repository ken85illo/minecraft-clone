#include "ChunkManager.hpp"
#include "Block/BlockType.hpp"
#include "Chunk.hpp"

void ChunkManager::updateMesh(Chunk& chunk, const MeshData& data, MeshType meshType) {
    chunk.m_meshData[meshType] = data;
}

void ChunkManager::uploadMesh(Chunk& chunk, MeshType meshType) {
    chunk.m_renderer.uploadMesh(chunk.m_meshData[meshType], meshType);
}

void ChunkManager::buildMesh(Chunk& chunk, const MeshData& data, MeshType meshType) {
    chunk.m_meshData[meshType] = data;
    chunk.m_renderer.uploadMesh(chunk.m_meshData[meshType], meshType);
}

void ChunkManager::updateBlock(Chunk& chunk, int32_t x, int32_t y, int32_t z, BlockType::Type blockType) {
    if(!chunk.m_blocks[x][y][z].isBreakable())
        return;

    MeshType meshType = chunk.m_blocks[x][y][z].isTransparent() ? TRANSPARENT : OPAQUE;
    MeshData (*generateMesh)(Chunk&) =
    (meshType == 1) ? ChunkMesh::buildTransparent : ChunkMesh::buildOpaque;

    chunk.m_blocks[x][y][z].setType(blockType);
    buildMesh(chunk, generateMesh(chunk), meshType);

    chunk.setHighestBlock(y + 2);
    updateNeighbour(chunk.m_leftChunk, x == 0, CHUNK_SIZE - 1, y, z);
    updateNeighbour(chunk.m_rightChunk, x == CHUNK_SIZE - 1, 0, y, z);
    updateNeighbour(chunk.m_backChunk, z == 0, x, y, CHUNK_SIZE - 1);
    updateNeighbour(chunk.m_frontChunk, z == CHUNK_SIZE - 1, x, y, 0);
}

void ChunkManager::updateNeighbour(Chunk* neighbourChunk, bool condition, int32_t x, int32_t y, int32_t z) {
    if(!neighbourChunk || !condition || neighbourChunk->isAirBlock(x, y, z))
        return;

    MeshType meshType =
    neighbourChunk->m_blocks[x][y][z].isTransparent() ? TRANSPARENT : OPAQUE;

    MeshData (*generateMesh)(Chunk&) =
    (meshType == 1) ? ChunkMesh::buildTransparent : ChunkMesh::buildOpaque;

    buildMesh(*neighbourChunk, generateMesh(*neighbourChunk), meshType);
}

void ChunkManager::deleteMesh(Chunk& chunk) {
    glDeleteBuffers(2, chunk.m_renderer.m_VBO);
    glDeleteBuffers(2, chunk.m_renderer.m_TBO);
    glDeleteBuffers(2, chunk.m_renderer.m_EBO);
    glDeleteVertexArrays(2, chunk.m_renderer.m_VAO);
}

void ChunkManager::render(Chunk& chunk, MeshType type) {
    chunk.m_renderer.render(type);
}
