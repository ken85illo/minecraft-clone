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

Block::Block(Type type, glm::vec3 min)
: m_type(type), m_rect{ min, min + 1.0f } {
}

void Block::setType(Type type) {
    m_type = type;
}

const std::array<float, 12> Block::getFace(uint8_t index) const {
    std::array<float, 12> posVertices;

    for(uint8_t i = 0; i < 12; i += 3) {
        posVertices[i] = s_vertices[index][i] + m_rect.min.x;
        posVertices[i + 1] = s_vertices[index][i + 1] + m_rect.min.y;
        posVertices[i + 2] = s_vertices[index][i + 2] + m_rect.min.z;
    }

    return posVertices;
}

const Block::Type& Block::getType() const {
    return m_type;
}
const Block::Rect& Block::getRect() const {
    return m_rect;
}
