#include "Chunk.hpp"
#include "World/Treeminator.hpp"

Player* Chunk::s_player = nullptr;

Chunk::Chunk(const std::array<std::array<float, CHUNK_SIZE>, CHUNK_SIZE>& heightMap,
glm::vec3 position)
: m_position(position.x - CHUNK_SIZE / 2.0f, 0.0f, position.z - CHUNK_SIZE / 2.0f),
  m_chunkBounds(m_position, m_position + glm::vec3(CHUNK_SIZE, MAX_HEIGHT, CHUNK_SIZE)) {

    for(int32_t x = 0; x < CHUNK_SIZE; x++)
        for(int32_t y = 0; y < MAX_HEIGHT; y++)
            for(int32_t z = 0; z < CHUNK_SIZE; z++) {
                int32_t height =
                floor(heightMap[x][z] * (TERRAIN_HEIGHT - TERRAIN_OFFSET)) + TERRAIN_OFFSET;
                int32_t waterHeight = WATER_LEVEL - TERRAIN_OFFSET;

                if(height > m_highestBlock || waterHeight > m_highestBlock)
                    m_highestBlock = std::max(height + 1, waterHeight + 1);

                glm::vec3 localPos = glm::vec3(x, y, z);
                Block::Type type = Block::AIR;

                if(y < height * 0.75)
                    type = Block::STONE_BLOCK;
                else if(y > height - 1 && y < waterHeight)
                    type = Block::WATER_BLOCK;
                else if(y >= height - SAND_DEPTH && y <= height - 1 && y < waterHeight)
                    type = Block::SAND_BLOCK;
                else if(y == height - 1) {
                    type = Block::GRASS_BLOCK;
                } else if(y < height)
                    type = Block::DIRT_BLOCK;

                m_blocks[x][y][z] = Block(type, localPos, localPos + m_position);
            }
}

void Chunk::setNeighbours(Chunk* front, Chunk* back, Chunk* right, Chunk* left) {
    m_frontChunk = front;
    m_backChunk = back;
    m_rightChunk = right;
    m_leftChunk = left;
}

Block* Chunk::getBlock(int32_t x, int32_t y, int32_t z) {
    Chunk* currentChunk = this;

    if(y < 0 || y >= MAX_HEIGHT)
        return nullptr;

    if(z >= CHUNK_SIZE) {
        if(!m_frontChunk)
            return nullptr;

        z -= CHUNK_SIZE;
        currentChunk = m_frontChunk;
    } else if(z < 0) {
        if(!m_backChunk)
            return nullptr;

        z += CHUNK_SIZE;
        currentChunk = m_backChunk;
    }

    if(x >= CHUNK_SIZE) {
        if(!m_rightChunk)
            return nullptr;

        x -= CHUNK_SIZE;
        currentChunk = m_rightChunk;
    } else if(x < 0) {
        if(!m_leftChunk)
            return nullptr;

        x += CHUNK_SIZE;
        currentChunk = m_leftChunk;
    }

    return &currentChunk->m_blocks[x][y][z];
}

bool Chunk::isAirBlock(int32_t x, int32_t y, int32_t z) {
    auto block = getBlock(x, y, z);
    return !block || block->getType() == Block::AIR;
}

void Chunk::spawnTrees() {
    Treeminator treeminator(this);
    for(int32_t x = 2; x < CHUNK_SIZE - 2; x++)
        for(int32_t y = 0; y < m_highestBlock; y++)
            for(int32_t z = 2; z < CHUNK_SIZE - 2; z++)
                treeminator.createTree(x, y, z);
}

void Chunk::setHighestBlock(uint8_t height) {
    if(height > m_highestBlock)
        m_highestBlock = height;
}

uint8_t Chunk::getHighestBlock() const {
    return m_highestBlock;
}

glm::vec3 Chunk::getPosition() const {
    return m_position;
}

const ChunkBounds& Chunk::getBounds() const {
    return m_chunkBounds;
}

Chunk* Chunk::getFrontChunk() const {
    return m_frontChunk;
}

Chunk* Chunk::getBackChunk() const {
    return m_backChunk;
}

Chunk* Chunk::getRightChunk() const {
    return m_rightChunk;
}

Chunk* Chunk::getLeftChunk() const {
    return m_leftChunk;
}

void Chunk::loadPlayer(Player* player) {
    s_player = player;
}
