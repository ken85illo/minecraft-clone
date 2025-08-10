#include "ChunkManager.hpp"
#include "Block/BlockType.hpp"
#include "Chunk.hpp"
#include "ChunkMesh.hpp"

void ChunkManager::updateMesh(Chunk& chunk, const MeshData& data, uint8_t index) {
    chunk.m_meshData[index] = data;
}

void ChunkManager::uploadMesh(Chunk& chunk, uint8_t index) {
    chunk.m_renderer.uploadMesh(chunk.m_meshData[index], index);
}

void ChunkManager::buildMesh(Chunk& chunk, const MeshData& data, uint8_t index) {
    chunk.m_meshData[index] = data;
    chunk.m_renderer.uploadMesh(chunk.m_meshData[index], index);
}

void ChunkManager::updateBlock(Chunk& chunk, int32_t x, int32_t y, int32_t z, int8_t type) {
    if(!chunk.m_blocks[x][y][z].isBreakable())
        return;

    int8_t index = chunk.m_blocks[x][y][z].isTransparent() ? 1 : 0;
    MeshData (*generateMesh)(Chunk&) =
    (index == 1) ? ChunkMesh::buildTransparent : ChunkMesh::buildOpaque;

    chunk.m_blocks[x][y][z].setType(static_cast<BlockType::Type>(type));
    buildMesh(chunk, generateMesh(chunk), index);

    chunk.setHighestBlock(y + 1);
    updateNeighbour(chunk.m_leftChunk, x == 0, CHUNK_SIZE - 1, y, z);
    updateNeighbour(chunk.m_rightChunk, x == CHUNK_SIZE - 1, 0, y, z);
    updateNeighbour(chunk.m_backChunk, z == 0, x, y, CHUNK_SIZE - 1);
    updateNeighbour(chunk.m_frontChunk, z == CHUNK_SIZE - 1, x, y, 0);
}

void ChunkManager::updateNeighbour(Chunk* neighbourChunk, bool condition, int32_t x, int32_t y, int32_t z) {
    if(!neighbourChunk || !condition || neighbourChunk->isAirBlock(x, y, z))
        return;

    int8_t index = neighbourChunk->m_blocks[x][y][z].isTransparent() ? 1 : 0;
    MeshData (*generateMesh)(Chunk&) =
    (index == 1) ? ChunkMesh::buildTransparent : ChunkMesh::buildOpaque;
    buildMesh(*neighbourChunk, generateMesh(*neighbourChunk), index);
}

void ChunkManager::deleteMesh(Chunk& chunk) {
    glDeleteBuffers(2, chunk.m_renderer.m_VBO);
    glDeleteBuffers(2, chunk.m_renderer.m_TBO);
    glDeleteBuffers(2, chunk.m_renderer.m_EBO);
    glDeleteVertexArrays(2, chunk.m_renderer.m_VAO);
}

void ChunkManager::render(Chunk& chunk, uint8_t index) {
    chunk.m_renderer.render(index);
}
