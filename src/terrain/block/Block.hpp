#pragma once

#include "texture/Atlas.hpp"
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

    Block(Type type, glm::vec3 min, glm::vec3 chunkPosition);

    // Setters
    void setType(Type type);

    // Getters
    const Type getType() const;
    const Rect& getLocalRect() const;
    const Rect& getGlobalRect() const;
    const std::pair<float*, float*> getTexCoord(uint8_t index) const;
    const std::pair<float*, float*> getFace(uint8_t index) const;

private:
    Type m_type;
    Rect m_localRect;
    Rect m_globalRect;

    static float s_vertices[6][12];
    static float s_texCoords[6][8];
    static Atlas s_atlas;
};
