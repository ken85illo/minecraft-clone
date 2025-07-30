#pragma once

#include <array>
#include <glm/glm.hpp>

class Block {
public:
    struct Rect {
        glm::vec3 min;
        glm::vec3 max;
    };

    enum Type {
        DIRT_BLOCK = 0,
        GRASS_BLOCK,
        STONE_BLOCK,
        OAK_WOOD_BLOCK,
        OAK_LEAF_BLOCK,
        AIR
    };

    Block(Type type, glm::vec3 min);

    // Setters
    void setType(Type type);

    // Getters
    const Type& getType() const;
    const Rect& getRect() const;
    const std::array<float, 12> getFace(uint8_t index) const;

private:
    Type m_type;
    Rect m_rect;

    static float s_vertices[6][12];
};
