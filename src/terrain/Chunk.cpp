#include "Chunk.hpp"

Chunk::Chunk(float heightMap[CHUNK_SIZE][CHUNK_SIZE], glm::vec3 position)
: m_position(position), m_highestBlock(0) {

    int32_t size = CHUNK_SIZE * MAX_HEIGHT * CHUNK_SIZE;
    m_blocks.reserve(size);

    uint8_t halfChunk = CHUNK_SIZE / 2;
    for(int32_t x = 0; x < CHUNK_SIZE; x++)
        for(int32_t y = 0; y < MAX_HEIGHT; y++)
            for(int32_t z = 0; z < CHUNK_SIZE; z++) {
                uint8_t height = 8 + floor(heightMap[x][z] * (TERRAIN_HEIGHT - 8));

                if(height > m_highestBlock)
                    m_highestBlock = height;

                glm::vec3 pos = glm::vec3(x - halfChunk, y, z - halfChunk);
                if(y >= height)
                    m_blocks.emplace_back(Block::Block::AIR, pos);
                else if(y == height - 1)
                    m_blocks.emplace_back(Block::GRASS_BLOCK, pos);
                else if(y < height * 0.75)
                    m_blocks.emplace_back(Block::STONE_BLOCK, pos);
                else if(y < height)
                    m_blocks.emplace_back(Block::DIRT_BLOCK, pos);
            }

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_vertVBO);
    glGenBuffers(1, &m_texVBO);
    glGenBuffers(1, &m_EBO);
}

void Chunk::deleteVertexArray() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &m_vertVBO);
    glDeleteBuffers(1, &m_texVBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void Chunk::setNeighbours(Chunk* front, Chunk* back, Chunk* right, Chunk* left) {
    m_frontChunk = front;
    m_backChunk = back;
    m_rightChunk = right;
    m_leftChunk = left;
}

void Chunk::generateMesh() {
    Atlas atlas = Atlas(m_texCoords);

    m_vertexData.clear();
    m_indexData.clear();
    m_textureData.clear();

    for(auto& index : m_indices)
        index %= 4;

    for(int32_t x = 0; x < CHUNK_SIZE; x++) {
        for(int32_t y = 0; y < MAX_HEIGHT; y++) {
            for(int32_t z = 0; z < CHUNK_SIZE; z++) {
                auto block = getType(x, y, z);
                if(block == Block::Block::AIR)
                    continue;

                atlas.map(block);
                fillFaces(x, y, z);
            }
        }
    }

    bindVertexArray();
}

void Chunk::fillFaces(int32_t x, int32_t y, int32_t z) {
    int32_t index = getIndex(x, y, z);

    int8_t dx[6] = { 0, 0, 0, 0, 1, -1 };
    int8_t dy[6] = { 0, 0, 1, -1, 0, 0 };
    int8_t dz[6] = { 1, -1, 0, 0, 0, 0 };


    for(uint8_t face = 0; face < 6; face++) {
        if(getType(x + dx[face], y + dy[face], z + dz[face]) != Block::Block::AIR)
            continue;

        auto& vertices = m_blocks[index].getFace(face);
        m_vertexData.insert(m_vertexData.end(), std::begin(vertices), std::end(vertices));
        m_textureData.insert(m_textureData.end(), std::begin(m_texCoords[face]),
        std::end(m_texCoords[face]));
        m_indexData.insert(m_indexData.end(), std::begin(m_indices), std::end(m_indices));

        for(auto& index : m_indices)
            index += 4;
    }
}

void Chunk::updateMesh(Block::Type type, int32_t x, int32_t y, int32_t z) {
    m_blocks[getIndex(x, y, z)].setType(type);
    if(y > m_highestBlock)
        m_highestBlock = y;

    generateMesh();

    if(m_leftChunk && x == 0)
        m_leftChunk->generateMesh();
    else if(m_rightChunk && x == CHUNK_SIZE - 1)
        m_rightChunk->generateMesh();

    if(m_backChunk && z == 0)
        m_backChunk->generateMesh();
    else if(m_frontChunk && z == CHUNK_SIZE - 1)
        m_frontChunk->generateMesh();
}


void Chunk::bindVertexArray() {
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexData.size() * sizeof(uint32_t),
    m_indexData.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertVBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(float),
    m_vertexData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_texVBO);
    glBufferData(GL_ARRAY_BUFFER, m_textureData.size() * sizeof(m_textureData[0]),
    m_textureData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

uint8_t Chunk::getHighestBlock() const {
    return m_highestBlock;
}

const Block::Type Chunk::getType(int32_t x, int32_t y, int32_t z) const {
    const Chunk* currentChunk = this;

    if(y < 0 || y >= MAX_HEIGHT)
        return Block::AIR;

    if(z >= CHUNK_SIZE) {
        if(!m_frontChunk)
            return Block::AIR;

        z -= CHUNK_SIZE;
        currentChunk = m_frontChunk;
    } else if(z < 0) {
        if(!m_backChunk)
            return Block::AIR;

        z += CHUNK_SIZE;
        currentChunk = m_backChunk;
    }

    if(x >= CHUNK_SIZE) {
        if(!m_rightChunk)
            return Block::AIR;

        x -= CHUNK_SIZE;
        currentChunk = m_rightChunk;
    } else if(x < 0) {
        if(!m_leftChunk)
            return Block::AIR;

        x += CHUNK_SIZE;
        currentChunk = m_leftChunk;
    }

    return currentChunk->m_blocks[getIndex(x, y, z)].getType();
}

int32_t Chunk::getIndex(int32_t x, int32_t y, int32_t z) const {
    return x * MAX_HEIGHT * CHUNK_SIZE + y * CHUNK_SIZE + z;
}

const Block::Rect& Chunk::getBlockRect(int32_t x, int32_t y, int32_t z) const {
    return m_blocks[getIndex(x, y, z)].getRect();
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

glm::vec3 Chunk::getPosition() const {
    return m_position;
}

void Chunk::render() {
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indexData.size(), GL_UNSIGNED_INT, nullptr);
}
