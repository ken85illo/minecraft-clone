#include "Chunk.hpp"
#include <print>

Chunk::Chunk(std::array<std::array<float, CHUNK_SIZE>, CHUNK_SIZE>& heightMap, glm::vec3 position)
: m_position(glm::vec3(position.x - CHUNK_SIZE / 2.0f, 0.0f, position.z - CHUNK_SIZE / 2.0f)),
  m_highestBlock(0) {

    int32_t size = CHUNK_SIZE * MAX_HEIGHT * CHUNK_SIZE;
    m_blocks.reserve(size);

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_vertVBO);
    glGenBuffers(1, &m_texVBO);
    glGenBuffers(1, &m_EBO);

    for(int32_t x = 0; x < CHUNK_SIZE; x++)
        for(int32_t y = 0; y < MAX_HEIGHT; y++)
            for(int32_t z = 0; z < CHUNK_SIZE; z++) {
                uint8_t height = 8 + floor(heightMap[x][z] * TERRAIN_HEIGHT);

                if(height > m_highestBlock)
                    m_highestBlock = height + 1;

                glm::vec3 localPos = glm::vec3(x, y, z);
                Block::Type type = Block::AIR;

                if(y == height - 1)
                    type = Block::GRASS_BLOCK;
                else if(y < height * 0.75)
                    type = Block::STONE_BLOCK;
                else if(y < height)
                    type = Block::DIRT_BLOCK;

                m_blocks.emplace_back(type, localPos, localPos + m_position);
            }
}

void Chunk::spawnTrees() {
    Treeminator treeminator = Treeminator(this);
    for(int32_t x = 2; x < CHUNK_SIZE - 2; x++)
        for(int32_t y = 0; y < m_highestBlock; y++)
            for(int32_t z = 2; z < CHUNK_SIZE - 2; z++)
                treeminator.createTree(x, y, z);
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

void Chunk::setHighestBlock(uint8_t height) {
    m_highestBlock = height;
}

void Chunk::generateMesh() {
    m_vertexData.clear();
    m_indexData.clear();
    m_textureData.clear();

    for(auto& index : m_indices)
        index %= 4;

    for(int32_t x = 0; x < CHUNK_SIZE; x++)
        for(int32_t y = 0; y < m_highestBlock; y++)
            for(int32_t z = 0; z < CHUNK_SIZE; z++) {
                if(isAirBlock(x, y, z))
                    continue;

                fillFaces(x, y, z);
            }
}

void Chunk::fillFaces(int32_t x, int32_t y, int32_t z) {
    int32_t index = getIndex(x, y, z);

    int8_t dx[6] = { 0, 0, 0, 0, 1, -1 };
    int8_t dy[6] = { 0, 0, 1, -1, 0, 0 };
    int8_t dz[6] = { 1, -1, 0, 0, 0, 0 };

    auto currentBlock = getBlock(x, y, z);

    for(uint8_t face = 0; face < 6; face++) {
        auto offsetBlock = getBlock(x + dx[face], y + dy[face], z + dz[face]);
        if(offsetBlock &&
        (offsetBlock->isTransparent() ? currentBlock->isTransparent() :
                                        offsetBlock->getType() != BlockType::AIR))
            continue;

        auto vertices = m_blocks[index].getFace(face);
        auto texCoords = m_blocks[index].getTexCoord(face);
        m_vertexData.insert(m_vertexData.end(), vertices.begin(), vertices.end());
        m_textureData.insert(m_textureData.end(), texCoords.first, texCoords.second);
        m_indexData.insert(m_indexData.end(), std::begin(m_indices), std::end(m_indices));

        for(auto& index : m_indices)
            index += 4;
    }
}

void Chunk::regenerateMesh() {
    generateMesh();
    bindVertexArray();
}

void Chunk::updateMesh(Block::Type type, int32_t x, int32_t y, int32_t z) {
    m_blocks[getIndex(x, y, z)].setType(type);
    if((y + 1) > m_highestBlock)
        m_highestBlock = y + 1;

    regenerateMesh();

    if(m_leftChunk && x == 0 && !m_leftChunk->isAirBlock(CHUNK_SIZE - 1, y, z))
        m_leftChunk->regenerateMesh();
    else if(m_rightChunk && x == CHUNK_SIZE - 1 && !m_rightChunk->isAirBlock(0, y, z))
        m_rightChunk->regenerateMesh();

    if(m_backChunk && z == 0 && !m_backChunk->isAirBlock(x, y, CHUNK_SIZE - 1))
        m_backChunk->regenerateMesh();
    else if(m_frontChunk && z == CHUNK_SIZE - 1 && !m_frontChunk->isAirBlock(x, y, 0))
        m_frontChunk->regenerateMesh();
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

    return &currentChunk->m_blocks[getIndex(x, y, z)];
}

bool Chunk::isAirBlock(int32_t x, int32_t y, int32_t z) {
    auto block = getBlock(x, y, z);
    return !block || block->getType() == Block::AIR;
}

int32_t Chunk::getIndex(int32_t x, int32_t y, int32_t z) const {
    return x * MAX_HEIGHT * CHUNK_SIZE + y * CHUNK_SIZE + z;
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
