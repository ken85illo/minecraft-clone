#include "Atlas.hpp"

Atlas::Atlas(uint16_t bitsPerTexture,
uint16_t totalBits,
glm::vec2 texCoords[6][4],
uint16_t textures[][6])
: m_bitsPerTexture(bitsPerTexture),
  m_totalBits(totalBits),
  m_texCoords(*texCoords),
  m_textures(*textures) {
}

std::unordered_map<std::string, uint16_t> Atlas::getMap(uint16_t side) {
    std::unordered_map<std::string, uint16_t> map;

    map["left"] = (side * m_bitsPerTexture) % m_totalBits;
    map["right"] = map["left"] + m_bitsPerTexture;
    map["top"] = m_totalBits - ((side * m_bitsPerTexture / m_totalBits) * m_bitsPerTexture);
    map["bottom"] = map["top"] - m_bitsPerTexture;

    return map;
}

void Atlas::map(uint16_t index) {
    static uint16_t lastIndex = -1;

    if(index == lastIndex)
        return;

    lastIndex = index;

    auto mfront = getMap(*(m_textures + index * 6));
    auto mback = getMap(*(m_textures + index * 6 + 1));
    auto mtop = getMap(*(m_textures + index * 6 + 2));
    auto mbottom = getMap(*(m_textures + index * 6 + 3));
    auto mright = getMap(*(m_textures + index * 6 + 4));
    auto mleft = getMap(*(m_textures + index * 6 + 5));

    glm::vec2 texMap[6][4] = {
        // front face
        glm::vec2(mfront["left"], mfront["bottom"]),  // bottom-left
        glm::vec2(mfront["right"], mfront["bottom"]), // bottom-right
        glm::vec2(mfront["right"], mfront["top"]),    // top-right
        glm::vec2(mfront["left"], mfront["top"]),     // top-left

        // back face
        glm::vec2(mback["right"], mback["bottom"]), // bottom-left
        glm::vec2(mback["left"], mback["bottom"]),  // bottom-right
        glm::vec2(mback["left"], mback["top"]),     // top-right
        glm::vec2(mback["right"], mback["top"]),    // top-left

        // top face
        glm::vec2(mtop["left"], mtop["bottom"]),  // bottom-left
        glm::vec2(mtop["right"], mtop["bottom"]), // bottom-right
        glm::vec2(mtop["right"], mtop["top"]),    // top-right
        glm::vec2(mtop["left"], mtop["top"]),     // top-left

        // bottom face
        glm::vec2(mbottom["right"], mbottom["bottom"]), // bottom-left
        glm::vec2(mbottom["left"], mbottom["bottom"]),  // bottom-right
        glm::vec2(mbottom["left"], mbottom["top"]),     // top-right
        glm::vec2(mbottom["right"], mbottom["top"]),    // top-left

        // right face
        glm::vec2(mright["left"], mright["bottom"]),  // bottom-left
        glm::vec2(mright["right"], mright["bottom"]), // bottom-right
        glm::vec2(mright["right"], mright["top"]),    // top-right
        glm::vec2(mright["left"], mright["top"]),     // top-left

        // left face
        glm::vec2(mleft["right"], mleft["bottom"]), // bottom-left
        glm::vec2(mleft["left"], mleft["bottom"]),  // bottom-right
        glm::vec2(mleft["left"], mleft["top"]),     // top-right
        glm::vec2(mleft["right"], mleft["top"]),    // top-left
    };

    for(int i = 0; i < 24; i++) {
        (m_texCoords + i)->x = (*texMap + i)->x / static_cast<float>(m_totalBits);
        (m_texCoords + i)->y = (*texMap + i)->y / static_cast<float>(m_totalBits);
    }
}
