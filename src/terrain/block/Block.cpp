#include "Block.hpp"

Block::Block(Type type, glm::vec3 min)
: m_type(type), m_rect{ min, min + 1.0f } {
    m_vertices = { {
    {
    // front face
    0.0f, 0.0f, 1.0f, // bottom-left
    1.0f, 0.0f, 1.0f, // bottom-right
    1.0f, 1.0f, 1.0f, // top-right
    0.0f, 1.0f, 1.0f, // top-left
    },

    {
    // back face
    1.0f, 0.0f, 0.0f, // bottom-left
    0.0f, 0.0f, 0.0f, // bottom-right
    0.0f, 1.0f, 0.0f, // top-right
    1.0f, 1.0f, 0.0f, // top-left
    },

    {
    // top face
    0.0f, 1.0f, 1.0f, // bottom-left
    1.0f, 1.0f, 1.0f, // bottom-right
    1.0f, 1.0f, 0.0f, // top-right
    0.0f, 1.0f, 0.0f, // top-left
    },

    {
    // bottom face
    1.0f, 0.0f, 1.0f, // bottom-left
    0.0f, 0.0f, 1.0f, // bottom-right
    0.0f, 0.0f, 0.0f, // top-right
    1.0f, 0.0f, 0.0f, // top-left
    },

    {
    // right face
    1.0f, 0.0f, 1.0f, // bottom-left
    1.0f, 0.0f, 0.0f, // bottom-right
    1.0f, 1.0f, 0.0f, // top-right
    1.0f, 1.0f, 1.0f, // top-left
    },

    {
    // left face
    0.0f, 0.0f, 0.0f, // bottom-left
    0.0f, 0.0f, 1.0f, // bottom-right
    0.0f, 1.0f, 1.0f, // top-right
    0.0f, 1.0f, 0.0f, // top-left
    },
    } };

    for(uint8_t i = 0; i < 6; i++) {
        for(uint8_t j = 0; j < 12; j += 3) {
            m_vertices[i][j] += min.x;
            m_vertices[i][j + 1] += min.y;
            m_vertices[i][j + 2] += min.z;
        }
    }
}

void Block::setType(Type type) {
    m_type = type;
}

const std::array<float, 12>& Block::getFace(uint8_t index) const {
    return m_vertices[index];
}

const Block::Type& Block::getType() const {
    return m_type;
}
const Block::Rect& Block::getRect() const {
    return m_rect;
}
