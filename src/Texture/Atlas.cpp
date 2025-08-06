#include "Atlas.hpp"

Atlas::Atlas(uint16_t textures[][6]) : m_textures(*textures) {
}

std::unordered_map<std::string, uint32_t> Atlas::getMap(uint16_t side) {
    std::unordered_map<std::string, uint32_t> map;

    map["left"] = (side * BITS_PER_TEXTURE) % TOTAL_BITS;
    map["right"] = map["left"] + BITS_PER_TEXTURE;
    map["top"] = TOTAL_BITS - ((side * BITS_PER_TEXTURE / TOTAL_BITS) * BITS_PER_TEXTURE);
    map["bottom"] = map["top"] - BITS_PER_TEXTURE;

    return map;
}

void Atlas::map(uint16_t index, float texCoords[6][8]) {
    auto mfront = getMap(*(m_textures + 6 * index));
    auto mback = getMap(*(m_textures + 6 * index + 1));
    auto mtop = getMap(*(m_textures + 6 * index + 2));
    auto mbottom = getMap(*(m_textures + 6 * index + 3));
    auto mright = getMap(*(m_textures + 6 * index + 4));
    auto mleft = getMap(*(m_textures + 6 * index + 5));

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

    for(int32_t i = 0; i < 6; i++)
        for(int32_t j = 0; j < 8; j++)
            texCoords[i][j] = texMap[i][j] / static_cast<float>(TOTAL_BITS);
}
