#pragma once

#include <array>
#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

#define BITS_PER_TEXTURE 16
#define TOTAL_BITS 256


class Atlas {
public:
    Atlas(uint16_t textures[][6], float texCoords[6][8]);
    void map(uint16_t index);

private:
    uint16_t* m_textures;
    float* m_texCoords;
    std::unordered_map<std::string, uint32_t> getMap(uint16_t side);
};
