#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

// convert variable name to string
#define GET_VARIABLE_NAME(variable) (#variable)

class Atlas {
public:
    Atlas(uint16_t bitsPerTexture, uint16_t totalBits, float* vertices);
    void map(uint16_t front, uint16_t back, uint16_t top, uint16_t bottom, uint16_t right, uint16_t left);

private:
    uint16_t m_bitsPerTexture;
    uint16_t m_totalBits;
    float* m_vertices;

    std::unordered_map<std::string, uint16_t> getMap(uint16_t side);
};
