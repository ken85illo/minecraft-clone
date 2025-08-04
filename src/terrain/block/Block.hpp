#pragma once

#include "BlockType.hpp"
#include <glm/glm.hpp>

class Block : public BlockType {
public:
    struct Rect {
        glm::vec3 min;
        glm::vec3 max;
    };

    Block(BlockType::Type type, glm::vec3 localPos, glm::vec3 globalPos);

    // Getters
    const Rect& getLocalRect() const;
    const Rect& getGlobalRect() const;
    const std::array<float, 12> getFace(uint8_t index) const;

private:
    Rect m_localRect;
    Rect m_globalRect;

    static float s_vertices[6][12];
    static BlockType s_blockType;
};
