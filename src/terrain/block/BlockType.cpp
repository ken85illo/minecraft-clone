#include "BlockType.hpp"
#include <print>

uint16_t BlockType::s_textures[5][6] = {
    { DIRT, DIRT, DIRT, DIRT, DIRT, DIRT }, // DIRT_BLOCK
    { GRASS_SIDE, GRASS_SIDE, GRASS_TOP, DIRT, GRASS_SIDE, GRASS_SIDE }, // GRASS_BLOCK
    { STONE, STONE, STONE, STONE, STONE, STONE }, // STONE BLOCK
    { OAK_SIDE, OAK_SIDE, OAK_TOP_BOTTOM, OAK_TOP_BOTTOM, OAK_SIDE, OAK_SIDE }, // OAK WOOD BLOCK
    { OAK_LEAF, OAK_LEAF, OAK_LEAF, OAK_LEAF, OAK_LEAF, OAK_LEAF }, // OAK LEAF BLOCK
};

Atlas BlockType::s_atlas = Atlas(s_textures);
float BlockType::s_textureTypes[BlockType::AIR][6][8];

BlockType::BlockType(Type type) : m_type(type) {
    switch(type) {
    case OAK_LEAF_BLOCK: m_transparent = true; break;
    default: m_transparent = false; break;
    };
}

void BlockType::setType(BlockType::Type type) {
    m_type = type;

    switch(type) {
    case OAK_LEAF_BLOCK: m_transparent = true; break;
    default: m_transparent = false; break;
    };
}

void BlockType::loadTextures() {
    for(uint8_t i = 0; i < AIR; i++)
        s_atlas.map(i, s_textureTypes[i]);
}

const std::pair<float*, float*> BlockType::getTexCoord(uint8_t index) const {
    return std::make_pair(
    s_textureTypes[m_type][index], (s_textureTypes[m_type][index] + 8));
}

const BlockType::Type BlockType::getType() const {
    return m_type;
}

const bool BlockType::isTransparent() const {
    return m_transparent;
}
