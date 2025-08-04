#include "BlockType.hpp"

uint16_t BlockType::s_textures[5][6] = {
    { DIRT, DIRT, DIRT, DIRT, DIRT, DIRT }, // DIRT_BLOCK
    { GRASS_SIDE, GRASS_SIDE, GRASS_TOP, DIRT, GRASS_SIDE, GRASS_SIDE }, // GRASS_BLOCK
    { STONE, STONE, STONE, STONE, STONE, STONE }, // STONE BLOCK
    { OAK_SIDE, OAK_SIDE, OAK_TOP_BOTTOM, OAK_TOP_BOTTOM, OAK_SIDE, OAK_SIDE }, // OAK WOOD BLOCK
    { OAK_LEAF, OAK_LEAF, OAK_LEAF, OAK_LEAF, OAK_LEAF, OAK_LEAF }, // OAK LEAF BLOCK
};

float BlockType::s_texCoords[6][8];
Atlas BlockType::s_atlas = Atlas(s_textures, s_texCoords);

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

const std::pair<float*, float*> BlockType::getTexCoord(uint8_t index) const {
    s_atlas.map(m_type);
    return std::make_pair(s_texCoords[index], (s_texCoords[index] + 8));
}

const BlockType::Type BlockType::getType() const {
    return m_type;
}

const bool BlockType::isTransparent() const {
    return m_transparent;
}
