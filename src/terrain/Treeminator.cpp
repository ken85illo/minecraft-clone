#include "Treeminator.hpp"
#include "Chunk.hpp"

Treeminator::Treeminator(Chunk* chunk) : m_chunk(chunk) {
}

void Treeminator::spawnTree(uint16_t x, uint16_t y, uint16_t z) {
    uint8_t chance = rand() % 100;

    if(m_chunk->getBlock(x, y, z)->getType() != BlockType::GRASS_BLOCK || chance > 1)
        return;

    std::stack<std::pair<Block*, BlockType::Type>> treeStack;

    for(int8_t ny = 1; ny <= 5; ny++) {
        auto currentBlock = m_chunk->getBlock(x, y + ny, z);
        if(currentBlock->getType() != BlockType::AIR)
            return;

        treeStack.push(std::make_pair(currentBlock, BlockType::OAK_WOOD_BLOCK));
    }

    for(int8_t nx = -2; nx <= 2; nx++)
        for(int8_t ny = 3; ny <= 4; ny++)
            for(int8_t nz = -2; nz <= 2; nz++) {
                auto currentBlock = m_chunk->getBlock(x + nx, y + ny, z + nz);

                if(currentBlock->getType() != BlockType::AIR)
                    return;

                if(std::abs(nx) == std::abs(nz))
                    continue;

                treeStack.push(std::make_pair(currentBlock, BlockType::OAK_LEAF_BLOCK));
            }

    for(int8_t nx = -1; nx <= 1; nx++)
        for(int8_t nz = -1; nz <= 1; nz++) {
            auto currentBlock = m_chunk->getBlock(x + nx, y + 5, z + nz);

            if(currentBlock->getType() != BlockType::AIR)
                return;

            if(nx == 0 && nz == 0)
                continue;

            treeStack.push(std::make_pair(currentBlock, BlockType::OAK_LEAF_BLOCK));
        }

    for(int8_t nx = -1; nx <= 1; nx++)
        for(int8_t nz = -1; nz <= 1; nz++) {
            auto currentBlock = m_chunk->getBlock(x + nx, y + 6, z + nz);

            if(currentBlock->getType() != BlockType::AIR)
                return;

            if(std::abs(nx) == std::abs(nz) && nx != 0 && nz != 0)
                continue;

            treeStack.push(std::make_pair(currentBlock, BlockType::OAK_LEAF_BLOCK));
        }

    while(!treeStack.empty()) {
        auto& [block, type] = treeStack.top();
        block->setType(type);
        treeStack.pop();
    }

    if(y + 7 > m_chunk->getHighestBlock())
        m_chunk->setHighestBlock(y + 7);
}
