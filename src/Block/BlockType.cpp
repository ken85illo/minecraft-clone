#include "BlockType.hpp"
#include <print>

uint16_t BlockType::s_textures[][6] = {
    { DIRT, DIRT, DIRT, DIRT, DIRT, DIRT }, // DIRT_BLOCK
    { GRASS_SIDE, GRASS_SIDE, GRASS_TOP, DIRT, GRASS_SIDE, GRASS_SIDE }, // GRASS_BLOCK
    { STONE, STONE, STONE, STONE, STONE, STONE }, // STONE BLOCK
    { OAK_SIDE, OAK_SIDE, OAK_TOP_BOTTOM, OAK_TOP_BOTTOM, OAK_SIDE, OAK_SIDE }, // OAK WOOD BLOCK
    { OAK_LEAF, OAK_LEAF, OAK_LEAF, OAK_LEAF, OAK_LEAF, OAK_LEAF }, // OAK LEAF BLOCK
    { SAND, SAND, SAND, SAND, SAND, SAND },       // SAND BLOCK
    { WATER, WATER, WATER, WATER, WATER, WATER }, // WATER BLOCK
};

Atlas BlockType::s_atlas = Atlas(s_textures);
float BlockType::s_textureTypes[BlockType::AIR][6][8];

BlockType::BlockType(Type type) : m_type(type) {
    initType(type);
}

void BlockType::initType(BlockType::Type type) {
    switch(type) {
    case WATER_BLOCK: m_breakable = false;
    case OAK_LEAF_BLOCK: m_transparent = true; break;
    default:
        m_transparent = false;
        m_breakable = true;
        break;
    };
}

void BlockType::setType(BlockType::Type type) {
    m_type = type;
    initType(type);
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

const bool BlockType::isBreakable() const {
    return m_breakable;
}
