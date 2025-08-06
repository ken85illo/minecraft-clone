#pragma once

#include "Texture/Atlas.hpp"
#include <cstdint>
#include <vector>

class BlockType {
public:
    enum Type {
        DIRT_BLOCK = 0,
        GRASS_BLOCK,
        STONE_BLOCK,
        OAK_WOOD_BLOCK,
        OAK_LEAF_BLOCK,
        AIR
    };

    BlockType(Type type);
    static void loadTextures();

    // Getters
    const std::pair<float*, float*> getTexCoord(uint8_t index) const;
    const BlockType::Type getType() const;
    const bool isTransparent() const;

    // Setters
    void setType(BlockType::Type type);

private:
    enum Texture {
        DIRT = 0,
        GRASS_SIDE,
        GRASS_TOP,
        STONE,
        OAK_SIDE,
        OAK_TOP_BOTTOM,
        OAK_LEAF
    };

    static Atlas s_atlas;
    static uint16_t s_textures[5][6];
    static float s_textureTypes[BlockType::AIR][6][8];

    Type m_type;
    bool m_transparent;
};
