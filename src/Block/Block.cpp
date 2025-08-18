#include "Block.hpp"
#include "BlockTexture.hpp"
#include "Chunk/Chunk.hpp"

float Block::s_vertices[6][12] = {
    // front face
    0.0f, 0.0f, 1.0f, // bottom-left
    1.0f, 0.0f, 1.0f, // bottom-right
    1.0f, 1.0f, 1.0f, // top-right
    0.0f, 1.0f, 1.0f, // top-left

    // back face
    1.0f, 0.0f, 0.0f, // bottom-left
    0.0f, 0.0f, 0.0f, // bottom-right
    0.0f, 1.0f, 0.0f, // top-right
    1.0f, 1.0f, 0.0f, // top-left

    // top face
    0.0f, 1.0f, 1.0f, // bottom-left
    1.0f, 1.0f, 1.0f, // bottom-right
    1.0f, 1.0f, 0.0f, // top-right
    0.0f, 1.0f, 0.0f, // top-left

    // bottom face
    1.0f, 0.0f, 1.0f, // bottom-left
    0.0f, 0.0f, 1.0f, // bottom-right
    0.0f, 0.0f, 0.0f, // top-right
    1.0f, 0.0f, 0.0f, // top-left

    // right face
    1.0f, 0.0f, 1.0f, // bottom-left
    1.0f, 0.0f, 0.0f, // bottom-right
    1.0f, 1.0f, 0.0f, // top-right
    1.0f, 1.0f, 1.0f, // top-left

    // left face
    0.0f, 0.0f, 0.0f, // bottom-left
    0.0f, 0.0f, 1.0f, // bottom-right
    0.0f, 1.0f, 1.0f, // top-right
    0.0f, 1.0f, 0.0f, // top-left
};

uint16_t Block::s_textures[][6] = {
    {       DIRT,       DIRT,           DIRT,           DIRT,       DIRT,       DIRT }, // DIRT_BLOCK
    { GRASS_SIDE, GRASS_SIDE,      GRASS_TOP,           DIRT, GRASS_SIDE, GRASS_SIDE }, // GRASS_BLOCK
    {      STONE,      STONE,          STONE,          STONE,      STONE,      STONE }, // STONE BLOCK
    {   OAK_SIDE,   OAK_SIDE, OAK_TOP_BOTTOM, OAK_TOP_BOTTOM,   OAK_SIDE,   OAK_SIDE }, // OAK WOOD BLOCK
    {   OAK_LEAF,   OAK_LEAF,       OAK_LEAF,       OAK_LEAF,   OAK_LEAF,   OAK_LEAF }, // OAK LEAF BLOCK
    {       SAND,       SAND,           SAND,           SAND,       SAND,       SAND }, // SAND BLOCK
    {      WATER,      WATER,          WATER,          WATER,      WATER,      WATER }, // WATER BLOCK
};

Atlas Block::s_atlas = Atlas(s_textures);
float Block::s_textureTypes[static_cast<size_t>(BlockType::AIR)][6][8];

void Block::loadTextures() {
    for (uint8_t i = 0; i < static_cast<uint8_t>(BlockType::AIR); i++) {
        s_atlas.map(i, s_textureTypes[i]);
    }
}

const BlockRect Block::getGlobalRect(Chunk &chunk, int32_t x, int32_t y, int32_t z) {
    glm::vec3 localPos = glm::vec3(x, y, z);
    return { chunk.getPosition() + localPos, chunk.getPosition() + localPos + 1.0f };
}

const BlockRect Block::getLocalRect(int32_t x, int32_t y, int32_t z) {
    glm::vec3 localPos = glm::vec3(x, y, z);
    return { localPos, localPos + 1.0f };
}

const std::array<float, 12> Block::getFace(BlockType &type, uint8_t face, int32_t x, int32_t y, int32_t z) {
    glm::vec3 localPos = getLocalRect(x, y, z).min;
    std::array<float, 12> posVertices;

    float decrement = (face == 2 && type == BlockType::WATER) ? 0.125f : 0.0f;
    for (uint8_t i = 0; i < 12; i += 3) {
        posVertices[i] = s_vertices[face][i] + localPos.x;
        posVertices[i + 1] = s_vertices[face][i + 1] + localPos.y - decrement;
        posVertices[i + 2] = s_vertices[face][i + 2] + localPos.z;
    }

    return posVertices;
}

const std::pair<float *, float *> Block::getTexCoord(BlockType &type, uint8_t face) {
    size_t typeIndex = static_cast<size_t>(type);
    return std::make_pair(s_textureTypes[typeIndex][face], (s_textureTypes[typeIndex][face] + 8));
}

const bool Block::isTransparent(BlockType &type) {
    switch (type) {
    case BlockType::OAK_LEAF:
    case BlockType::WATER:
        return true;
    default:
        return false;
    };
}

const bool Block::isBreakable(BlockType &type) {
    switch (type) {
    case BlockType::WATER:
        return false;
    default:
        return true;
    };
}
