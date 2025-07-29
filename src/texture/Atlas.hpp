#pragma once

#include <array>
#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

#define BITS_PER_TEXTURE 16
#define TOTAL_BITS 256

enum BlockTexture {
    DIRT = 0,
    GRASS_SIDE,
    GRASS_TOP,
    STONE,
    OAK_SIDE,
    OAK_TOP_BOTTOM,
    OAK_LEAF
};

class Atlas {
public:
    Atlas(float texCoords[6][8]);
    void map(uint16_t index);

private:
    float* m_texCoords;
    static uint16_t m_textures[5][6];
    std::unordered_map<std::string, uint32_t> getMap(uint16_t side);
};
