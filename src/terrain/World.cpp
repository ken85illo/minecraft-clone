#include "World.hpp"

World::World(Player* player)
: m_diameter(WORLD_RADIUS * 2 + 1),
  m_perlinNoise(FREQUENCY, AMPLITUDE, PERMUTATION_SIZE, NUMBER_OF_OCTAVES),
  m_player(player) {

    std::array<std::array<float, CHUNK_SIZE>, CHUNK_SIZE> heightMap;
    for(int32_t chunkX = -WORLD_RADIUS; chunkX <= WORLD_RADIUS; chunkX++) {
        for(int32_t chunkZ = -WORLD_RADIUS; chunkZ <= WORLD_RADIUS; chunkZ++) {
            generateHeightMap(heightMap, chunkX + WORLD_RADIUS, chunkZ + WORLD_RADIUS);

            m_chunks.emplace_back(heightMap,
            glm::vec3(chunkX * CHUNK_SIZE, 0.0f, chunkZ * CHUNK_SIZE));
        }
    }

    for(int32_t chunkX = 0; chunkX < m_diameter; chunkX++) {
        for(int32_t chunkZ = 0; chunkZ < m_diameter; chunkZ++) {
            m_chunkThreads.push_back(
            std::async(std::launch::async, [chunkX, chunkZ, this]() {
                Chunk* currentChunk = getChunk(chunkX, chunkZ);

                currentChunk->setNeighbours(getChunk(chunkX, chunkZ + 1),
                getChunk(chunkX, chunkZ - 1), getChunk(chunkX + 1, chunkZ),
                getChunk(chunkX - 1, chunkZ));

                currentChunk->spawnTrees();
                currentChunk->generateMesh();
            }));
        }
    }

    player->setSpawn(this);

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

const int32_t World::getDiameter() const {
    return m_diameter;
}

const int32_t World::getIndex(int32_t x, int32_t z) const {
    return x * m_diameter + z;
}

Chunk* World::getChunk(int32_t x, int32_t z) {
    if(x < 0 || x >= m_diameter || z < 0 || z >= m_diameter)
        return nullptr;

    int32_t index = getIndex(x, z);
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

void World::drawChunk(int32_t index, uint8_t renderIndex, Shader* shader) {
    if(m_chunkThreads[index].valid()) {
        m_chunkThreads[index].get();
        m_chunks[index].bindVertexArray(0);
        m_chunks[index].bindVertexArray(1);
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_chunks[index].getPosition());
    shader->setMat4("model", model);

    m_chunks[index].render(renderIndex);
}


void World::render(bool wireFrameMode, Shader* worldShader, Shader* lineShader) {
    lineShader->use();
    lineShader->setVec3("color", glm::vec3(0.2f, 0.5f, 0.5f));

    Shader* currentShader = (wireFrameMode) ? lineShader : worldShader;
    currentShader->use();
    m_texture->bind(0);

    const ChunkCoord& chunkCoord = m_player->getChunkCoord();
    int32_t mid = getIndex(chunkCoord.chunkX, chunkCoord.chunkZ);

    for(uint8_t renderIndex = 0; renderIndex < 2; ++renderIndex) {
        int32_t i = 0;
        int32_t j = m_chunks.size() - 1;

        while(i >= 0 && i < mid || j > mid && j < m_chunks.size()) {
            if(i < mid)
                drawChunk(i++, renderIndex, currentShader);
            if(j > mid)
                drawChunk(j--, renderIndex, currentShader);
        }

        if(mid >= 0 && mid < m_chunks.size())
            drawChunk(mid, renderIndex, currentShader);
    }
}
