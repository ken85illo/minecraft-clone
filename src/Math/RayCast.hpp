#pragma once

#include <cstdint>
#include <glm/glm.hpp>

class Player;
class Camera;
class Chunk;
class Block;

struct RayCoords {
    Chunk* chunk;
    uint32_t x, y, z;
};

class RayCast {
public:
    RayCast(float rangeRadius, Player* player);
    RayCoords sendRay();

    RayCoords getCoordsAtPoint(glm::vec3 point);
    const glm::vec3 getRayOrigin() const;
    const glm::vec3 getRayDirection() const;

private:
    float m_rangeRadius;
    Player* m_player;
};
