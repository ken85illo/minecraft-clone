#include "ChunkMesh.hpp"
#include "Player/Player.hpp"
#include <map>

MeshData ChunkMesh::buildOpaque(Chunk& chunk) {
    MeshData mesh;

    for(int32_t x = 0; x < CHUNK_SIZE; x++)
        for(int32_t y = 0; y <= chunk.m_highestBlock; y++)
            for(int32_t z = 0; z < CHUNK_SIZE; z++) {
                auto& block = chunk.m_blocks[x][y][z];
                if(block == BlockType::AIR || Block::isTransparent(block))
                    continue;

                fillFaces(chunk, x, y, z, mesh);
            }

    return mesh;
}

MeshData ChunkMesh::buildTransparent(Chunk& chunk) {
    MeshData mesh;
    std::multimap<float, std::tuple<uint8_t, uint8_t, uint8_t>> transparentBlocks;

    for(int32_t x = 0; x < CHUNK_SIZE; x++)
        for(int32_t y = 0; y <= chunk.m_highestBlock; y++)
            for(int32_t z = 0; z < CHUNK_SIZE; z++) {
                auto& block = chunk.m_blocks[x][y][z];
                if(!Block::isTransparent(block))
                    continue;

                float distance = glm::length(Chunk::s_player->getPosition() - Block::getGlobalRect(chunk, x, y, z).min);
                transparentBlocks.emplace(distance, std::make_tuple(x, y, z));
            }


    for(auto i = transparentBlocks.rbegin(); i != transparentBlocks.rend(); ++i)
        fillFaces(chunk, std::get<0>(i->second), std::get<1>(i->second), std::get<2>(i->second), mesh);

    return mesh;
}

void ChunkMesh::fillFaces(Chunk& chunk, int32_t x, int32_t y, int32_t z, MeshData& mesh) {
    static const int8_t dx[6] = { 0, 0, 0, 0, 1, -1 };
    static const int8_t dy[6] = { 0, 0, 1, -1, 0, 0 };
    static const int8_t dz[6] = { 1, -1, 0, 0, 0, 0 };

    auto& currentBlock = chunk.m_blocks[x][y][z];

    static const std::vector<uint32_t> indicesTemplate = { 0, 1, 3, 1, 2, 3 };
    static const float reciprocal = 1 / 3.0f;

    for(uint8_t face = 0; face < 6; face++) {
        auto offsetBlock = chunk.getBlockType(x + dx[face], y + dy[face], z + dz[face]);
        if(!offsetBlock || (Block::isTransparent(*offsetBlock) ? Block::isTransparent(currentBlock) : *offsetBlock != BlockType::AIR))
            continue;

        auto vertices = Block::getFace(currentBlock, face, x, y, z);
        auto texCoords = Block::getTexCoord(currentBlock, face);

        mesh.vertices.insert(mesh.vertices.end(), vertices.begin(), vertices.end());
        mesh.texCoords.insert(mesh.texCoords.end(), texCoords.first, texCoords.second);

        uint32_t baseIndex = mesh.vertices.size() * reciprocal - 4;
        for(auto idx : indicesTemplate)
            mesh.indices.push_back(baseIndex + idx);
    }
}
