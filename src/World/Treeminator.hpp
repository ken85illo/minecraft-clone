#pragma once

#include "Block/Block.hpp"

class Chunk;

// epic name
class Treeminator {
public:
    static void createTree(Chunk &chunk, int32_t x, int32_t y, int32_t z);

private:
    static bool addWood(
        Chunk &chunk, std::stack<std::pair<BlockType *, BlockType>> &treeStack, uint8_t height, uint16_t x, uint16_t y,
        uint16_t z
    );

    static bool addLeafLayer(
        Chunk &chunk, std::stack<std::pair<BlockType *, BlockType>> &treeStack, uint8_t radius, uint8_t height,
        uint16_t x, uint16_t y, uint16_t z, bool removeEdge, bool removeCenter
    );
};
