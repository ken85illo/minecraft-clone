#include "Chunk.hpp"

Chunk::Chunk(int16_t chunkSize, int16_t height, int16_t maxHeight, glm::vec3 pos)
: chunkSize(chunkSize), m_position(pos), height(height), maxHeight(maxHeight) {

    uint16_t size = chunkSize * maxHeight * chunkSize;
    blocks.reserve(size);

    for(int x = 0; x < chunkSize; x++)
        for(int y = 0; y < maxHeight; y++)
            for(int z = 0; z < chunkSize; z++)
                blocks.emplace_back((y < height) ? SOLID : AIR);


    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_vertVBO);
    glGenBuffers(1, &m_texVBO);
    glGenBuffers(1, &m_EBO);
}

void Chunk::generateMesh(Chunk* front, Chunk* back, Chunk* right, Chunk* left) {
    glm::vec3 vertices[][4] = {
        // front face
        glm::vec3(-0.5f, -0.5f, 0.5f), // bottom-left
        glm::vec3(0.5f, -0.5f, 0.5f),  // bottom-right
        glm::vec3(0.5f, 0.5f, 0.5f),   // top-right
        glm::vec3(-0.5f, 0.5f, 0.5f),  // top-left

        // back face
        glm::vec3(0.5f, -0.5f, -0.5f),  // bottom-left
        glm::vec3(-0.5f, -0.5f, -0.5f), // bottom-right
        glm::vec3(-0.5f, 0.5f, -0.5f),  // top-right
        glm::vec3(0.5f, 0.5f, -0.5f),   // top-left

        // top face
        glm::vec3(-0.5f, 0.5f, 0.5f),  // bottom-left
        glm::vec3(0.5f, 0.5f, 0.5f),   // bottom-right
        glm::vec3(0.5f, 0.5f, -0.5f),  // top-right
        glm::vec3(-0.5f, 0.5f, -0.5f), // top-left

        // bottom face
        glm::vec3(0.5f, -0.5f, 0.5f),   // bottom-left
        glm::vec3(-0.5f, -0.5f, 0.5f),  // bottom-right
        glm::vec3(-0.5f, -0.5f, -0.5f), // top-right
        glm::vec3(0.5f, -0.5f, -0.5f),  // top-left

        // right face
        glm::vec3(0.5f, -0.5f, 0.5f),  // bottom-left
        glm::vec3(0.5f, -0.5f, -0.5f), // bottom-right
        glm::vec3(0.5f, 0.5f, -0.5f),  // top-right
        glm::vec3(0.5f, 0.5f, 0.5f),   // top-left

        // left face
        glm::vec3(-0.5f, -0.5f, -0.5f), // bottom-left
        glm::vec3(-0.5f, -0.5f, 0.5f),  // bottom-right
        glm::vec3(-0.5f, 0.5f, 0.5f),   // top-right
        glm::vec3(-0.5f, 0.5f, -0.5f),  // top-left
    };

    uint32_t indices[6] = {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    glm::vec2 texCoords[6][4];
    uint16_t textures[][6] = {
        { DIRT, DIRT, DIRT, DIRT, DIRT, DIRT },                              //
        { GRASS_SIDE, GRASS_SIDE, GRASS_TOP, DIRT, GRASS_SIDE, GRASS_SIDE }, //
    };
    Atlas atlas{ 16, 256, texCoords, textures };

    int16_t halfChunk = chunkSize / 2.0f;

    for(int x = 0; x < chunkSize; x++) {
        for(int y = 0; y < maxHeight; y++) {
            for(int z = 0; z < chunkSize; z++) {
                if(getType(x, y, z) == AIR)
                    continue;

                if(getType(x, y + 1, z) == AIR)
                    atlas.map(GRASS_BLOCK);
                else
                    atlas.map(DIRT);

                int16_t dx[6] = { 0, 0, 0, 0, 1, -1 };
                int16_t dy[6] = { 0, 0, 1, -1, 0, 0 };
                int16_t dz[6] = { 1, -1, 0, 0, 0, 0 };

                for(int face = 0; face < 6; face++) {
                    if(getType(x + dx[face], y + dy[face], z + dz[face], front,
                       back, right, left) != AIR)
                        continue;

                    for(auto& point : vertices[face])
                        vertexData.emplace_back(point +
                        glm::vec3(x - halfChunk, y - chunkSize, z - halfChunk));

                    for(auto& point : texCoords[face])
                        textureData.emplace_back(point);

                    for(auto& index : indices) {
                        indexData.push_back(index);
                        index += 4;
                    }
                }
            }
        }
    }

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
    indexData.size() * sizeof(indexData[0]), indexData.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertVBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(vertexData[0]),
    vertexData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, m_texVBO);
    glBufferData(GL_ARRAY_BUFFER, textureData.size() * sizeof(textureData[0]),
    textureData.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

BlockType
Chunk::getType(int16_t x, int16_t y, int16_t z, Chunk* front, Chunk* back, Chunk* right, Chunk* left) const {
    if(y < 0 || y >= maxHeight)
        return AIR;

    if(z >= chunkSize) {
        if(!front)
            return AIR;

        return front->blocks[getIndex(x, y, 0)];
    }

    if(z < 0) {
        if(!back)
            return AIR;

        return back->blocks[getIndex(x, y, back->chunkSize - 1)];
    }

    if(x >= chunkSize) {
        if(!right)
            return AIR;

        return right->blocks[getIndex(0, y, z)];
    }

    if(x < 0) {
        if(!left)
            return AIR;

        return left->blocks[getIndex(left->chunkSize - 1, y, z)];
    }

    return blocks[x * maxHeight * chunkSize + y * chunkSize + z];
}

uint16_t Chunk::getIndex(uint16_t x, uint16_t y, uint16_t z) const {
    return x * maxHeight * chunkSize + y * chunkSize + z;
}

void Chunk::render() {
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, vertexData.size() * 3, GL_UNSIGNED_INT, nullptr);
}

glm::vec3 Chunk::getPosition() const {
    return m_position;
}
