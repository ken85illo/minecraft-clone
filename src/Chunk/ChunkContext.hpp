#pragma once

#include <glm/glm.hpp>

struct ChunkCoords {
    int32_t x;
    int32_t z;

    bool operator==(const ChunkCoords &other) const {
        return x == other.x && z == other.z;
    }
};

struct ChunkBounds {
    glm::vec3 min;
    glm::vec3 max;
};

constexpr int32_t CHUNK_SIZE = 16;
constexpr int32_t MAX_HEIGHT = 256;
constexpr int32_t TERRAIN_HEIGHT = 128;

constexpr int32_t WATER_LEVEL = 72;
constexpr int32_t TERRAIN_OFFSET = 8;
constexpr int32_t SAND_DEPTH = 5;
