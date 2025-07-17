#include "World.hpp"

World::World(uint8_t radius, Shader* shader)
: m_shader(shader), m_size(radius * 2 + 1) {

    uint8_t chunkSize = 16;
    uint8_t height = 8;
    for(int x = -radius; x <= radius; x++)
        for(int z = -radius; z <= radius; z++) {
            m_chunks.emplace_back(
            16, height, height, glm::vec3(x * chunkSize, 0.0f, z * chunkSize));
        }


    for(int x = 0; x < m_size; x++) {
        for(int z = 0; z < m_size; z++) {
            m_chunks[x * m_size + z].generateMesh(getChunk(x, z + 1),
            getChunk(x, z - 1), getChunk(x + 1, z), getChunk(x - 1, z));
        }
    }

    m_texture = new Texture(GL_TEXTURE_2D, 1);
    m_texture->bind(0);
    m_texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    m_texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_texture->setParameter(GL_TEXTURE_MAX_LEVEL, 3);
    m_texture->loadImage("../res/atlas.png");

    m_shader->use();
    m_shader->setInt("texture0", 0);
}

Chunk* World::getChunk(uint16_t x, uint16_t z) {
    if(x < 0 || x >= m_size || z < 0 || z >= m_size)
        return nullptr;

    uint16_t index = x * m_size + z;
    return &m_chunks[index];
}

void World::render() {
    for(auto& chunk : m_chunks) {
        m_shader->use();
        m_texture->bind(0);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, chunk.getPosition());
        m_shader->setMat4("model", model);

        chunk.render();
    }
}
