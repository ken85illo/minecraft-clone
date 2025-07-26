#include "Chunk.hpp"

Chunk::Chunk(float heightMap[CHUNK_SIZE][CHUNK_SIZE], glm::vec3 pos)
: position(pos), highestBlock(0) {

    int32_t size = CHUNK_SIZE * MAX_HEIGHT * CHUNK_SIZE;
    blocks.reserve(size);


    for(int32_t x = 0; x < CHUNK_SIZE; x++)
        for(int32_t y = 0; y < MAX_HEIGHT; y++)
            for(int32_t z = 0; z < CHUNK_SIZE; z++) {
                int32_t height = 8 + floor(heightMap[x][z] * 64);

                if(height > highestBlock)
                    highestBlock = height;

                if(y >= height)
                    blocks.emplace_back(AIR);
                else if(y == height - 1)
                    blocks.emplace_back(GRASS_BLOCK);
                else if(y < height / 2)
                    blocks.emplace_back(STONE_BLOCK);
                else if(y < height)
                    blocks.emplace_back(DIRT_BLOCK);
            }


    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_vertVBO);
    glGenBuffers(1, &m_texVBO);
    glGenBuffers(1, &m_EBO);
}

void Chunk::setNeighbours(Chunk* front, Chunk* back, Chunk* right, Chunk* left) {
    this->front = front;
    this->back = back;
    this->right = right;
    this->left = left;
}

void Chunk::generateMesh() {
    Atlas atlas = Atlas(m_texCoords, m_textures);

    m_vertexData.clear();
    m_indexData.clear();
    m_textureData.clear();

    for(auto& index : m_indices)
        index %= 4;

    for(int32_t x = 0; x < CHUNK_SIZE; x++) {
        for(int32_t y = 0; y < MAX_HEIGHT; y++) {
            for(int32_t z = 0; z < CHUNK_SIZE; z++) {
                auto block = getType(x, y, z);
                if(block == AIR)
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

    float halfChunk = CHUNK_SIZE / 2.0f;

    for(int face = 0; face < 6; face++) {
        if(getType(x + dx[face], y + dy[face], z + dz[face]) != AIR)
            continue;

        float vertices[12];
        for(int i = 0; i < 12; i += 3) {
            vertices[i] = m_vertices[face][i] + x - halfChunk + 0.5f;
            vertices[i + 1] = m_vertices[face][i + 1] + y;
            vertices[i + 2] = m_vertices[face][i + 2] + z - halfChunk + 0.5f;
        }
        m_vertexData.insert(m_vertexData.end(), std::begin(vertices), std::end(vertices));
        m_textureData.insert(m_textureData.end(), std::begin(m_texCoords[face]),
        std::end(m_texCoords[face]));
        m_indexData.insert(m_indexData.end(), std::begin(m_indices), std::end(m_indices));

        for(auto& index : m_indices)
            index += 4;
    }
}

void Chunk::updateMesh(BlockType type, int32_t x, int32_t y, int32_t z) {
    blocks[getIndex(x, y, z)] = type;
    if(y > highestBlock)
        highestBlock = y;

    generateMesh();

    if(left && x == 0)
        left->generateMesh();
    else if(left && x == CHUNK_SIZE - 1)
        right->generateMesh();

    if(back && z == 0)
        back->generateMesh();
    else if(front && z == CHUNK_SIZE - 1)
        front->generateMesh();
}


void Chunk::bindVertexArray() {
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
    m_indexData.size() * sizeof(m_indexData[0]), m_indexData.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertVBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(m_vertexData[0]),
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

BlockType Chunk::getType(int32_t x, int32_t y, int32_t z) const {
    if(y < 0 || y >= MAX_HEIGHT)
        return AIR;

    if(z >= CHUNK_SIZE) {
        if(!front)
            return AIR;

        return front->getType(x, y, 0);
    }

    if(z < 0) {
        if(!back)
            return AIR;

        return back->getType(x, y, CHUNK_SIZE - 1);
    }

    if(x >= CHUNK_SIZE) {
        if(!right)
            return AIR;

        return right->getType(0, y, z);
    }

    if(x < 0) {
        if(!left)
            return AIR;

        return left->getType(CHUNK_SIZE - 1, y, z);
    }

    return blocks[getIndex(x, y, z)];
}

int32_t Chunk::getIndex(int32_t x, int32_t y, int32_t z) const {
    return x * MAX_HEIGHT * CHUNK_SIZE + y * CHUNK_SIZE + z;
}

BlockRect Chunk::getBlockRect(int32_t x, int32_t y, int32_t z) const {
    float halfChunk = CHUNK_SIZE / 2.0f;
    glm::vec3 blockPos = position + glm::vec3(-halfChunk, 0.0f, -halfChunk) +
    glm::vec3(x + 0.5f, y + 0.5f, z + 0.5f);

    return BlockRect{
        .position = blockPos,              //
        .min = blockPos - glm::vec3(0.5f), //
        .max = blockPos + glm::vec3(0.5f)  //
    };
}
void Chunk::render() {
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indexData.size(), GL_UNSIGNED_INT, nullptr);
}
