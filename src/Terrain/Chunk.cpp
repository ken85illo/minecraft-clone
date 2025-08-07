#include "Chunk.hpp"
#include "Core/Player/Player.hpp"

Player* Chunk::s_player = nullptr;

Chunk::Chunk(std::array<std::array<float, CHUNK_SIZE>, CHUNK_SIZE>& heightMap, glm::vec3 position)
: m_position(glm::vec3(position.x - CHUNK_SIZE / 2.0f, 0.0f, position.z - CHUNK_SIZE / 2.0f)),
  m_chunkBounds(m_position, m_position + glm::vec3(CHUNK_SIZE, MAX_HEIGHT, CHUNK_SIZE)),
  m_highestBlock(0) {
    glGenVertexArrays(2, m_VAO);
    glGenBuffers(2, m_vertVBO);
    glGenBuffers(2, m_texVBO);
    glGenBuffers(2, m_EBO);

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

                m_blocks[x][y][z] = Block(type, localPos, localPos + m_position);
            }
}

void Chunk::spawnTrees() {
    Treeminator treeminator = Treeminator(this);
    for(int32_t x = 2; x < CHUNK_SIZE - 2; x++)
        for(int32_t y = 0; y < m_highestBlock; y++)
            for(int32_t z = 2; z < CHUNK_SIZE - 2; z++)
                treeminator.createTree(x, y, z);
}


void Chunk::loadPlayer(Player* player) {
    s_player = player;
}

void Chunk::deleteVertexArray() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(2, m_vertVBO);
    glDeleteBuffers(2, m_texVBO);
    glDeleteBuffers(2, m_EBO);
    glDeleteVertexArrays(2, m_VAO);
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
    m_transparentBlocks.clear();

    for(int32_t x = 0; x < CHUNK_SIZE; x++)
        for(int32_t y = 0; y < m_highestBlock; y++)
            for(int32_t z = 0; z < CHUNK_SIZE; z++) {
                if(isAirBlock(x, y, z))
                    continue;

                if(m_blocks[x][y][z].isTransparent()) {
                    m_transparentBlocks.emplace_back(x, y, z);
                    continue;
                }

                fillFaces(x, y, z, 0);
            }

    sortTransparent();
    m_indexCount[0] = m_indexData[0].size();
    m_indexCount[1] = m_indexData[1].size();
}

void Chunk::sortTransparent() {
    std::sort(m_transparentBlocks.begin(), m_transparentBlocks.end(),
    [this](std::tuple<uint8_t, uint8_t, uint8_t>& a, std::tuple<uint8_t, uint8_t, uint8_t>& b) {
        auto& first =
        m_blocks[std::get<0>(a)][std::get<1>(a)][std::get<2>(a)].getGlobalRect().min;
        auto& second =
        m_blocks[std::get<0>(b)][std::get<1>(b)][std::get<2>(b)].getGlobalRect().min;

        return glm::length(s_player->getPosition() - first) >
        glm::length(s_player->getPosition() - second);
    });

    for(auto& block : m_transparentBlocks)
        fillFaces(std::get<0>(block), std::get<1>(block), std::get<2>(block), 1);
}

void Chunk::fillFaces(int32_t x, int32_t y, int32_t z, uint8_t index) {
    int8_t dx[6] = { 0, 0, 0, 0, 1, -1 };
    int8_t dy[6] = { 0, 0, 1, -1, 0, 0 };
    int8_t dz[6] = { 1, -1, 0, 0, 0, 0 };

    auto& currentBlock = m_blocks[x][y][z];

    for(uint8_t face = 0; face < 6; face++) {
        auto offsetBlock = getBlock(x + dx[face], y + dy[face], z + dz[face]);
        if(!offsetBlock ||
        (offsetBlock->isTransparent() ? currentBlock.isTransparent() :
                                        offsetBlock->getType() != BlockType::AIR))
            continue;

        auto vertices = currentBlock.getFace(face);
        auto texCoords = currentBlock.getTexCoord(face);

        m_vertexData[index].insert(
        m_vertexData[index].end(), vertices.begin(), vertices.end());
        m_textureData[index].insert(
        m_textureData[index].end(), texCoords.first, texCoords.second);
        m_indexData[index].insert(m_indexData[index].end(),
        std::begin(m_indices[index]), std::end(m_indices[index]));

        for(auto& index : m_indices[index])
            index += 4;
    }
}

void Chunk::regenerateMesh() {
    generateMesh();
    bindVertexArray(0);
    bindVertexArray(1);
}

void Chunk::generateTransparent() {
    Chunk* chunks[] = {
        this,
        m_leftChunk,
        m_rightChunk,
        m_backChunk,
        m_frontChunk,
    };

    for(uint8_t i = 0; i < std::size(chunks); i++) {
        if(chunks[i]) {
            chunks[i]->sortTransparent();
            chunks[i]->bindVertexArray(1);
        }
    }
}

void Chunk::updateMesh(Block::Type type, int32_t x, int32_t y, int32_t z) {
    m_blocks[x][y][z].setType(type);
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


void Chunk::bindVertexArray(uint8_t index) {
    glBindVertexArray(m_VAO[index]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO[index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexData[index].size() * sizeof(uint32_t),
    m_indexData[index].data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertVBO[index]);
    glBufferData(GL_ARRAY_BUFFER, m_vertexData[index].size() * sizeof(float),
    m_vertexData[index].data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_texVBO[index]);
    glBufferData(GL_ARRAY_BUFFER, m_textureData[index].size() * sizeof(float),
    m_textureData[index].data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_vertexData[index].clear();
    m_textureData[index].clear();
    m_indexData[index].clear();

    for(auto& val : m_indices[index])
        val %= 4;
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

    return &currentChunk->m_blocks[x][y][z];
}

bool Chunk::isAirBlock(int32_t x, int32_t y, int32_t z) {
    auto block = getBlock(x, y, z);
    return !block || block->getType() == Block::AIR;
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

const ChunkBounds& Chunk::getBounds() const {
    return m_chunkBounds;
}

void Chunk::render(uint8_t index) {
    if(index == 1)
        glDisable(GL_CULL_FACE);

    glBindVertexArray(m_VAO[index]);
    glDrawElements(GL_TRIANGLES, m_indexCount[index], GL_UNSIGNED_INT, 0);

    glEnable(GL_CULL_FACE);
}
