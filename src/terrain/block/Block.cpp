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

float Block::s_texCoords[6][8];
Atlas Block::s_atlas = Atlas(Block::s_texCoords);

Block::Block(Type type, glm::vec3 min, glm::vec3 chunkPosition)
: m_type(type),
  m_localRect{ min, min + 1.0f },
  m_globalRect{ min + chunkPosition, min + 1.0f + chunkPosition } {
}

void Block::setType(Type type) {
    m_type = type;
}

const std::pair<float*, float*> Block::getTexCoord(uint8_t index) const {
    s_atlas.map(m_type);
    return std::make_pair(s_texCoords[index], (s_texCoords[index] + 8));
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

const Block::Type Block::getType() const {
    return m_type;
}

const Block::Rect& Block::getLocalRect() const {
    return m_localRect;
}

const Block::Rect& Block::getGlobalRect() const {
    return m_globalRect;
}
