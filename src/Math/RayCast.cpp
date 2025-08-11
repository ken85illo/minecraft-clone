#include "RayCast.hpp"
#include "Chunk/Chunk.hpp"
#include "Player/Player.hpp"

RayCast::RayCast(float rangeRadius, Player* player) : m_rangeRadius(rangeRadius), m_player(player) {
}

const glm::vec3 RayCast::getRayOrigin() const {
    return m_player->getPosition() + m_player->getFront() * 0.1f;
}

const glm::vec3 RayCast::getRayDirection() const {
    return m_player->getFront();
}

RayCoords RayCast::sendRay() {
    glm::vec3 rayOrigin = getRayOrigin();
    glm::vec3 rayDirection = getRayDirection();

    float length = 0.1f;

    glm::vec3 point;
    while(length <= floor(m_rangeRadius / 2.0f)) {
        point = rayOrigin + length * rayDirection;
        RayCoords coords = getCoordsAtPoint(point);

        if(!coords.chunk)
            break;

        Block* block = coords.chunk->getBlock(coords.x, coords.y, coords.z);
        if(!block || block->getType() == BlockType::AIR) {
            length += 0.1f;
            continue;
        }

        return coords;
    }

    return RayCoords(nullptr, 0, 0, 0);
}

RayCoords RayCast::getCoordsAtPoint(glm::vec3 point) {
    Chunk* currentChunk = m_player->getCurrentChunk();

    if(!currentChunk || point.y < 0.0f || point.y >= MAX_HEIGHT)
        return RayCoords(nullptr, 0, 0, 0);

    uint8_t halfChunk = CHUNK_SIZE / 2;
    float dx = point.x - currentChunk->getPosition().x; // leftmost
    float dz = point.z - currentChunk->getPosition().z; // backmost

    if(dx < 0) {
        currentChunk = (currentChunk->getLeftChunk()) ? currentChunk->getLeftChunk() : currentChunk;
        dx += CHUNK_SIZE;
    } else if(dx >= CHUNK_SIZE) {
        currentChunk = (currentChunk->getRightChunk()) ? currentChunk->getRightChunk() : currentChunk;
        dx -= CHUNK_SIZE;
    }

    if(dz < 0) {
        currentChunk = (currentChunk->getBackChunk()) ? currentChunk->getBackChunk() : currentChunk;
        dz += CHUNK_SIZE;
    } else if(dz >= CHUNK_SIZE) {
        currentChunk = (currentChunk->getFrontChunk()) ? currentChunk->getFrontChunk() : currentChunk;
        dz -= CHUNK_SIZE;
    }

    uint16_t nx = floor(dx);
    uint16_t ny = floor(point.y);
    uint16_t nz = floor(dz);

    return RayCoords(currentChunk, nx, ny, nz);
}
