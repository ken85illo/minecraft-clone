#include "Treeminator.hpp"
#include "Chunk.hpp"

Treeminator::Treeminator(Chunk* chunk) : m_chunk(chunk) {
}

void Treeminator::spawnTree(uint16_t x, uint16_t y, uint16_t z) {
    uint8_t chance = rand() % 100;

    if(m_chunk->getBlock(x, y, z)->getType() != BlockType::GRASS_BLOCK || chance > 1)
        return;

    std::stack<std::pair<Block*, BlockType::Type>> treeStack;

    if(!addWood(treeStack, 5, x, y, z))
        return;


    uint8_t radius[4] = { 2, 2, 1, 1 };
    uint8_t removeEdge[4] = { true, true, false, true };
    uint8_t removeCenter[4] = { true, true, true, false };

    for(uint8_t height = 3; height <= 6; height++) {
        uint8_t index = height - 3;
        if(!addLeafLayer(treeStack, radius[index], height, x, y, z,
           removeEdge[index], removeCenter[index]))
            return;
    }

    while(!treeStack.empty()) {
        auto& [block, type] = treeStack.top();
        block->setType(type);
        treeStack.pop();
    }

    if(y + 7 > m_chunk->getHighestBlock())
        m_chunk->setHighestBlock(y + 7);
}

bool Treeminator::addWood(std::stack<std::pair<Block*, BlockType::Type>>& treeStack,
uint8_t height,
uint16_t x,
uint16_t y,
uint16_t z) {
    for(int8_t ny = 1; ny <= height; ny++) {
        auto currentBlock = m_chunk->getBlock(x, y + ny, z);

        if(currentBlock->getType() != BlockType::AIR)
            return false;


        treeStack.push(std::make_pair(currentBlock, BlockType::OAK_WOOD_BLOCK));
    }
    return true;
}

bool Treeminator::addLeafLayer(std::stack<std::pair<Block*, BlockType::Type>>& treeStack,
uint8_t radius,
uint8_t height,
uint16_t x,
uint16_t y,
uint16_t z,
bool removeEdge,
bool removeCenter) {

    for(int8_t nx = -radius; nx <= radius; nx++)
        for(int8_t nz = -radius; nz <= radius; nz++) {
            auto currentBlock = m_chunk->getBlock(x + nx, y + height, z + nz);

            if(currentBlock->getType() != BlockType::AIR)
                return false;

            bool edgeCase =
            (removeEdge) ? std::abs(nx) == std::abs(nz) && nx != 0 && nz != 0 : removeEdge;

            bool centerCase = (removeCenter) ? nx == 0 && nz == 0 : removeCenter;

            if(edgeCase || centerCase)
                continue;

            treeStack.push(std::make_pair(currentBlock, BlockType::OAK_LEAF_BLOCK));
        }

    return true;
}
