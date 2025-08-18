#include "ChunkManager.hpp"
#include "Chunk.hpp"

void ChunkManager::updateMesh(Chunk &chunk, const MeshData &data, MeshType meshType) {
    size_t index = static_cast<size_t>(meshType);
    chunk.m_meshData[index] = data;
}

void ChunkManager::uploadMesh(Chunk &chunk, MeshType meshType) {
    if (!chunk.m_renderer) {
        chunk.m_renderer = std::make_unique<ChunkRenderer>();
    }

    size_t index = static_cast<size_t>(meshType);
    chunk.m_renderer->uploadMesh(chunk.m_meshData[index], meshType);
}

void ChunkManager::buildMesh(Chunk &chunk) {
    updateMesh(chunk, ChunkMesh::buildOpaque(chunk), MeshType::OPAQUE);
    updateMesh(chunk, ChunkMesh::buildTransparent(chunk), MeshType::TRANSPARENT);
    uploadMesh(chunk, MeshType::OPAQUE);
    uploadMesh(chunk, MeshType::TRANSPARENT);
}

void ChunkManager::updateBlock(Chunk &chunk, int32_t x, int32_t y, int32_t z, BlockType blockType) {
    if (!Block::isBreakable(chunk.m_blocks[x][y][z])) {
        return;
    }

    chunk.m_blocks[x][y][z] = blockType;
    chunk.setHighestBlock(y);
    buildMesh(chunk);

    updateNeighbour(chunk.m_leftChunk, x == 0, CHUNK_SIZE - 1, y, z);
    updateNeighbour(chunk.m_rightChunk, x == CHUNK_SIZE - 1, 0, y, z);
    updateNeighbour(chunk.m_backChunk, z == 0, x, y, CHUNK_SIZE - 1);
    updateNeighbour(chunk.m_frontChunk, z == CHUNK_SIZE - 1, x, y, 0);
}

void ChunkManager::updateNeighbour(Chunk *neighbourChunk, bool condition, int32_t x, int32_t y, int32_t z) {
    if (!neighbourChunk || !condition || *neighbourChunk->getBlockType(x, y, z) == BlockType::AIR) {
        return;
    }

    neighbourChunk->setHighestBlock(y);
    buildMesh(*neighbourChunk);
}

void ChunkManager::render(Chunk &chunk, MeshType type) {
    chunk.m_renderer->render(type);
}
