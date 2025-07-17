#pragma once

#include <array>
#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

class Atlas {
public:
    Atlas(uint16_t bitsPerTexture,
    uint16_t totalBits,
    glm::vec2 texCoords[6][4],
    uint16_t textures[][6]);

    void map(uint16_t index);

private:
    uint16_t m_bitsPerTexture;
    uint16_t m_totalBits;
    glm::vec2* m_texCoords;
    uint16_t* m_textures;

    std::unordered_map<std::string, uint16_t> getMap(uint16_t side);
};
