#include "Atlas.hpp"

Atlas::Atlas(uint16_t bitsPerTexture, uint16_t totalBits, float* vertices)
: m_bitsPerTexture(bitsPerTexture), m_totalBits(totalBits), m_vertices(vertices) {
}

std::unordered_map<std::string, uint16_t> Atlas::getMap(uint16_t side) {
    std::unordered_map<std::string, uint16_t> map;

    map["left"] = (side * m_bitsPerTexture) % m_totalBits;
    map["right"] = map["left"] + m_bitsPerTexture;
    map["top"] = m_totalBits - ((side * m_bitsPerTexture / m_totalBits) * m_bitsPerTexture);
    map["bottom"] = map["top"] - m_bitsPerTexture;

    return map;
}

void Atlas::map(uint16_t front, uint16_t back, uint16_t top, uint16_t bottom, uint16_t right, uint16_t left) {
    auto mfront = getMap(front);
    auto mback = getMap(back);
    auto mtop = getMap(top);
    auto mbottom = getMap(bottom);
    auto mright = getMap(right);
    auto mleft = getMap(left);

    uint16_t texMap[] = {
        // front face
        mfront["left"], mfront["bottom"],  // bottom-left
        mfront["left"], mfront["top"],     // top-left
        mfront["right"], mfront["top"],    // top-right
        mfront["right"], mfront["bottom"], // bottom-right

        // back face
        mback["right"], mback["bottom"], // bottom-left
        mback["right"], mback["top"],    // top-left
        mback["left"], mback["top"],     // top-right
        mback["left"], mback["bottom"],  // bottom-right

        // top face
        mtop["left"], mtop["bottom"],  // bottom-left
        mtop["left"], mtop["top"],     // top-left
        mtop["right"], mtop["top"],    // top-right
        mtop["right"], mtop["bottom"], // bottom-right

        // bottom face
        mbottom["right"], mbottom["bottom"], // bottom-left
        mbottom["right"], mbottom["top"],    // top-left
        mbottom["left"], mbottom["top"],     // top-right
        mbottom["left"], mbottom["bottom"],  // bottom-right

        // right face
        mright["left"], mright["bottom"],  // bottom-left
        mright["left"], mright["top"],     // top-left
        mright["right"], mright["top"],    // top-right
        mright["right"], mright["bottom"], // bottom-right

        // left face
        mleft["right"], mleft["bottom"], // bottom-left
        mleft["right"], mleft["top"],    // top-left
        mleft["left"], mleft["top"],     // top-right
        mleft["left"], mleft["bottom"],  // bottom-right
    };

    for(int i = 0, j = 3; i < std::size(texMap); i++, j += 5) {
        int temp = j;
        while(i < std::size(texMap) && j < temp + 2) {
            *(m_vertices + j) = texMap[i] / static_cast<float>(m_totalBits);
            i++;
            j++;
        }
        i--;
        j = temp;
    }
}
