#pragma once

#include <cstdint>
#include <stack>

class Chunk;

// epic name
class Treeminator {
public:
    Treeminator(Chunk* chunk);
    void spawnTree(uint16_t x, uint16_t y, uint16_t z);

private:
    Chunk* m_chunk;
};
