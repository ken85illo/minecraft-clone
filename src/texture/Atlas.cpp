#include "Atlas.hpp"

uint16_t Atlas::m_textures[5][6] = {
    { DIRT, DIRT, DIRT, DIRT, DIRT, DIRT }, // DIRT_BLOCK
    { GRASS_SIDE, GRASS_SIDE, GRASS_TOP, DIRT, GRASS_SIDE, GRASS_SIDE }, // GRASS_BLOCK
    { STONE, STONE, STONE, STONE, STONE, STONE }, // STONE BLOCK
    { OAK_SIDE, OAK_SIDE, OAK_TOP_BOTTOM, OAK_TOP_BOTTOM, OAK_SIDE, OAK_SIDE }, // OAK WOOD BLOCK
    { OAK_LEAF, OAK_LEAF, OAK_LEAF, OAK_LEAF, OAK_LEAF, OAK_LEAF }, // OAK LEAF BLOCK
};

Atlas::Atlas(float texCoords[6][8]) : m_texCoords(*texCoords) {
}

std::unordered_map<std::string, uint32_t> Atlas::getMap(uint16_t side) {
    std::unordered_map<std::string, uint32_t> map;

    map["left"] = (side * BITS_PER_TEXTURE) % TOTAL_BITS;
    map["right"] = map["left"] + BITS_PER_TEXTURE;
    map["top"] = TOTAL_BITS - ((side * BITS_PER_TEXTURE / TOTAL_BITS) * BITS_PER_TEXTURE);
    map["bottom"] = map["top"] - BITS_PER_TEXTURE;

    return map;
}

void Atlas::map(uint16_t index) {
    static uint16_t lastIndex = -1;

    if(index == lastIndex)
        return;

    lastIndex = index;

    auto mfront = getMap(m_textures[index][0]);
    auto mback = getMap(m_textures[index][1]);
    auto mtop = getMap(m_textures[index][2]);
    auto mbottom = getMap(m_textures[index][3]);
    auto mright = getMap(m_textures[index][4]);
    auto mleft = getMap(m_textures[index][5]);

    uint32_t texMap[6][8] = {
        // front face
        mfront["left"], mfront["bottom"],  // bottom-left
        mfront["right"], mfront["bottom"], // bottom-right
        mfront["right"], mfront["top"],    // top-right
        mfront["left"], mfront["top"],     // top-left

        // back face
        mback["right"], mback["bottom"], // bottom-left
        mback["left"], mback["bottom"],  // bottom-right
        mback["left"], mback["top"],     // top-right
        mback["right"], mback["top"],    // top-left

        // top face
        mtop["left"], mtop["bottom"],  // bottom-left
        mtop["right"], mtop["bottom"], // bottom-right
        mtop["right"], mtop["top"],    // top-right
        mtop["left"], mtop["top"],     // top-left

        // bottom face
        mbottom["right"], mbottom["bottom"], // bottom-left
        mbottom["left"], mbottom["bottom"],  // bottom-right
        mbottom["left"], mbottom["top"],     // top-right
        mbottom["right"], mbottom["top"],    // top-left

        // right face
        mright["left"], mright["bottom"],  // bottom-left
        mright["right"], mright["bottom"], // bottom-right
        mright["right"], mright["top"],    // top-right
        mright["left"], mright["top"],     // top-left

        // left face
        mleft["right"], mleft["bottom"], // bottom-left
        mleft["left"], mleft["bottom"],  // bottom-right
        mleft["left"], mleft["top"],     // top-right
        mleft["right"], mleft["top"],    // top-left
    };

    for(int i = 0; i < 48; i++) {
        *(m_texCoords + i) = *(*texMap + i) / static_cast<float>(TOTAL_BITS);
        *(m_texCoords + i) = *(*texMap + i) / static_cast<float>(TOTAL_BITS);
    }
}
