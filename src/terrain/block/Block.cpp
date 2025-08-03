#include "Block.hpp"

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


Block::Block(BlockType::Type type, glm::vec3 localPos, glm::vec3 globalPos)
: BlockType(type),
  m_localRect{ localPos, localPos + 1.0f },
  m_globalRect{ globalPos, globalPos + 1.0f } {
}

const std::pair<float*, float*> Block::getFace(uint8_t index) const {
    static std::array<float, 12> posVertices;

    for(uint8_t i = 0; i < 12; i += 3) {
        posVertices[i] = s_vertices[index][i] + m_localRect.min.x;
        posVertices[i + 1] = s_vertices[index][i + 1] + m_localRect.min.y;
        posVertices[i + 2] = s_vertices[index][i + 2] + m_localRect.min.z;
    }

    return std::make_pair(posVertices.data(), posVertices.data() + posVertices.max_size());
}

const Block::Rect& Block::getLocalRect() const {
    return m_localRect;
}

const Block::Rect& Block::getGlobalRect() const {
    return m_globalRect;
}
