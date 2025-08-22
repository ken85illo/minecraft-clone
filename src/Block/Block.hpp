#pragma once

#include "BlockData.hpp"
#include "Texture/Atlas.hpp"

class Chunk;
enum BlockTexture : uint8_t;

class Block {
public:
    static void loadTextures();

    static const bool isTransparent(BlockType &type);
    static const bool isBreakable(BlockType &type);
    static const BlockRect getLocalRect(int32_t x, int32_t y, int32_t z);
    static const BlockRect getGlobalRect(Chunk &chunk, int32_t x, int32_t y, int32_t z);
    static const std::array<float, 24> getFace(BlockType &type, uint8_t face, int32_t x, int32_t y, int32_t z);
    static const std::pair<float *, float *> getTexCoord(BlockType &type, uint8_t face);

private:
    static float s_vertices[6][24];
    static Atlas s_atlas;
    static uint8_t s_textures[][6];
    static float s_textureTypes[static_cast<size_t>(BlockType::AIR)][6][8];
};
