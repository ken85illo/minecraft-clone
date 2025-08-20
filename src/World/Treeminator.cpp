#include "Treeminator.hpp"
#include "Chunk/Chunk.hpp"

void Treeminator::createTree(Chunk &chunk, int32_t x, int32_t y, int32_t z) {
    uint8_t chance = rand() % 100;

    if (chance > 1) {
        return;
    }

    std::stack<std::pair<BlockType *, BlockType>> treeStack;
    if (!addWood(chunk, treeStack, 5, x, y, z)) {
        return;
    }

    uint8_t radius[4] = { 2, 2, 1, 1 };
    uint8_t removeEdge[4] = { true, true, false, true };
    uint8_t removeCenter[4] = { true, true, true, false };

    for (uint8_t i = 0; i < 4; i++) {
        uint8_t height = i + 3;
        if (!addLeafLayer(chunk, treeStack, radius[i], height, x, y, z, removeEdge[i], removeCenter[i])) {
            return;
        }
    }

    while (!treeStack.empty()) {
        auto &[block, type] = treeStack.top();
        *block = type;
        treeStack.pop();
    }

    chunk.setHighestBlock(y + 6);
}

bool Treeminator::addWood(
    Chunk &chunk, std::stack<std::pair<BlockType *, BlockType>> &treeStack, uint8_t height, uint16_t x, uint16_t y,
    uint16_t z
) {
    for (int8_t ny = 1; ny <= height; ny++) {
        auto currentBlock = chunk.getBlockType(x, y + ny, z);

        if (*currentBlock != BlockType::AIR) {
            return false;
        }

        treeStack.push(std::make_pair(currentBlock, BlockType::OAK_WOOD));
    }
    return true;
}

bool Treeminator::addLeafLayer(
    Chunk &chunk, std::stack<std::pair<BlockType *, BlockType>> &treeStack, uint8_t radius, uint8_t height, uint16_t x,
    uint16_t y, uint16_t z, bool removeEdge, bool removeCenter
) {
    for (int8_t nx = -radius; nx <= radius; nx++) {
        for (int8_t nz = -radius; nz <= radius; nz++) {
            auto currentBlock = chunk.getBlockType(x + nx, y + height, z + nz);

            if (*currentBlock != BlockType::AIR) {
                return false;
            }

            int8_t ax = std::abs(nx);
            int8_t az = std::abs(nz);

            bool edgeCase = (removeEdge) ? ax == az && ax == radius && az == radius : removeEdge;
            bool centerCase = (removeCenter) ? nx == 0 && nz == 0 : removeCenter;

            if (edgeCase || centerCase) {
                continue;
            }

            treeStack.push(std::make_pair(currentBlock, BlockType::OAK_LEAF));
        }
    }

    return true;
}
