#pragma once

#define BITS_PER_TEXTURE 16
#define TOTAL_BITS 256

class Atlas {
public:
    Atlas(uint16_t textures[][6]);
    void map(uint16_t index, float texCoords[6][8]);

private:
    uint16_t *m_textures;
    std::unordered_map<std::string, float> getMap(uint16_t side);
};
