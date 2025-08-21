#include "ChunkManager.hpp"
#include "Chunk.hpp"

void ChunkManager::updateMesh(Chunk &chunk, const MeshData &data, MeshType meshType) {
    size_t index = static_cast<size_t>(meshType);
    chunk.m_meshData[index] = data;
}

void ChunkManager::uploadMesh(Chunk &chunk, MeshType meshType) {
    chunk.m_renderer.init();
    size_t index = static_cast<size_t>(meshType);
    chunk.m_renderer.uploadMesh(chunk.m_meshData[index], meshType);
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

const std::string ChunkManager::getBinaryName(int32_t chunkX, int32_t chunkZ) {
    std::stringstream stream;
    stream << "chunk_" << chunkX << "_" << chunkZ;
    return stream.str();
}

void ChunkManager::serialize(Chunk &chunk, int32_t chunkX, int32_t chunkZ) {
    using namespace std::literals::string_literals;

    std::filesystem::path dir = "./data/world/";
    if (!std::filesystem::exists(dir)) {
        std::filesystem::create_directory(dir);
    }

    const std::string filepath = dir.string() + getBinaryName(chunkX, chunkZ) + ".dat"s;
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::println("Error: Cant open chunk data file!");
        return;
    }

    file.write(reinterpret_cast<const char *>(&chunk.m_position), sizeof(chunk.m_position));
    file.write(reinterpret_cast<const char *>(&chunk.m_chunkBounds), sizeof(chunk.m_chunkBounds));
    file.write(reinterpret_cast<const char *>(chunk.m_blocks), sizeof(chunk.m_blocks));
    file.write(reinterpret_cast<const char *>(&chunk.m_highestBlock), sizeof(chunk.m_highestBlock));
    file.close();
}

bool ChunkManager::binaryExists(int32_t chunkX, int32_t chunkZ) {
    using namespace std::literals::string_literals;

    const std::string filepath = "./data/world/"s + getBinaryName(chunkX, chunkZ) + ".dat"s;
    return std::filesystem::exists(filepath);
}

Chunk *ChunkManager::deserialize(int32_t chunkX, int32_t chunkZ) {
    using namespace std::literals::string_literals;

    std::filesystem::path dir = "./data/world/";
    if (!std::filesystem::exists(dir)) {
        std::println("Error: Directory does not exist!");
        return nullptr;
    }

    const std::string filepath = dir.string() + getBinaryName(chunkX, chunkZ) + ".dat"s;
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        std::println("Error: Cant open chunk data file!");
        return nullptr;
    }

    Chunk *chunk = new Chunk;
    file.read(reinterpret_cast<char *>(&chunk->m_position), sizeof(chunk->m_position));
    file.read(reinterpret_cast<char *>(&chunk->m_chunkBounds), sizeof(chunk->m_chunkBounds));
    file.read(reinterpret_cast<char *>(chunk->m_blocks), sizeof(chunk->m_blocks));
    file.read(reinterpret_cast<char *>(&chunk->m_highestBlock), sizeof(chunk->m_highestBlock));
    file.close();

    return chunk;
}
void ChunkManager::render(Chunk &chunk, MeshType type) {
    chunk.m_renderer.render(type);
}
