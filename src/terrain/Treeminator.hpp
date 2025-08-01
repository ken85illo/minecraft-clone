#pragma once

#include "terrain/block/Block.hpp"
#include <cstdint>
#include <stack>

class Chunk;

// epic name
class Treeminator {
public:
    Treeminator(Chunk* chunk);
    void spawnTree(uint16_t x, uint16_t y, uint16_t z);

    bool addWood(std::stack<std::pair<Block*, BlockType::Type>>& treeStack,
    uint8_t height,
    uint16_t x,
    uint16_t y,
    uint16_t z);

    bool addLeafLayer(std::stack<std::pair<Block*, BlockType::Type>>& treeStack,
    uint8_t radius,
    uint8_t height,
    uint16_t x,
    uint16_t y,
    uint16_t z,
    bool removeEdge,
    bool removeCenter);

private:
    Chunk* m_chunk;
};
