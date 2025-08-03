#include "World.hpp"

World::World(Player* player)
: m_size(WORLD_RADIUS * 2 + 1),
  m_perlinNoise(FREQUENCY, AMPLITUDE, PERMUTATION_SIZE, NUMBER_OF_OCTAVES) {

    std::array<std::array<float, CHUNK_SIZE>, CHUNK_SIZE> heightMap;
    for(int32_t chunkX = -WORLD_RADIUS; chunkX <= WORLD_RADIUS; chunkX++)
        for(int32_t chunkZ = -WORLD_RADIUS; chunkZ <= WORLD_RADIUS; chunkZ++) {
            generateHeightMap(heightMap, chunkX + WORLD_RADIUS, chunkZ + WORLD_RADIUS);

            m_chunks.emplace_back(glm::vec3(chunkX * CHUNK_SIZE, 0.0f, chunkZ * CHUNK_SIZE));
            m_chunks.back().initChunk(heightMap);
        }

    for(int32_t chunkX = 0; chunkX < m_size; chunkX++) {
        for(int32_t chunkZ = 0; chunkZ < m_size; chunkZ++) {
            Chunk* currentChunk = getChunk(chunkX, chunkZ);

            currentChunk->setNeighbours(getChunk(chunkX, chunkZ + 1),
            getChunk(chunkX, chunkZ - 1), getChunk(chunkX + 1, chunkZ),
            getChunk(chunkX - 1, chunkZ));

            currentChunk->generateMesh();
        }
    }

    player->setCurrentChunk(getChunk(m_size / 2, m_size / 2));

    m_texture = new Texture(GL_TEXTURE_2D, 1);
    m_texture->bind(0);
    m_texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    m_texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_texture->setParameter(GL_TEXTURE_MAX_LEVEL, 3);
    m_texture->loadImage("../res/atlas.png");
}

World::~World() {
    for(auto& chunk : m_chunks)
        chunk.deleteVertexArray();

    delete m_texture;
}

Chunk* World::getChunk(uint16_t x, uint16_t z) {
    if(x < 0 || x >= m_size || z < 0 || z >= m_size)
        return nullptr;

    uint16_t index = x * m_size + z;
    return &m_chunks[index];
}


void World::generateHeightMap(std::array<std::array<float, CHUNK_SIZE>, CHUNK_SIZE>& heightMap,
int32_t chunkX,
int32_t chunkZ) {
    for(int32_t x = 0; x < CHUNK_SIZE; x++) {
        for(int32_t z = 0; z < CHUNK_SIZE; z++) {
            float n = m_perlinNoise.fractalBrownianMotion(
            (chunkX * CHUNK_SIZE + x), (chunkZ * CHUNK_SIZE + z));

            n += 1.0f;
            n /= 2.0f;

            heightMap[x][z] = n;
        }
    }
}


void World::render(bool wireFrameMode, Shader* worldShader, Shader* lineShader) {
    static Shader* currentShader;

    lineShader->setVec3("color", glm::vec3(0.2f, 0.5f, 0.5f));
    currentShader = (wireFrameMode) ? lineShader : worldShader;
    currentShader->use();

    m_texture->bind(0);
    for(auto& chunk : m_chunks) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, chunk.getPosition());
        currentShader->setMat4("model", model);

        chunk.render();
    }
}
