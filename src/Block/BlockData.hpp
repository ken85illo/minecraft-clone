#pragma once

struct BlockRect {
    glm::vec3 min;
    glm::vec3 max;
};

enum class BlockType : uint8_t {
    DIRT = 0,
    GRASS,
    STONE,
    OAK_WOOD,
    OAK_LEAF,
    SAND,
    WATER,
    AIR // Must be the last block
};
