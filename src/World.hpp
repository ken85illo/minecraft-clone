#pragma once

#include "Chunk.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <cstdint>

class World {
public:
    World(uint8_t radius, Shader* shader);
    void render();

    Chunk* getChunk(uint16_t x, uint16_t z);

private:
    std::vector<Chunk> m_chunks;
    Shader* m_shader;
    Texture* m_texture;
    uint8_t m_textureIndex;
    uint16_t m_size;
};
