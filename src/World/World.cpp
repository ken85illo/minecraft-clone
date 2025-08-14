#include "World.hpp"
#include "Player/Player.hpp"
#include "TerrainGenerator.hpp"
#include "Utils/Timer.hpp"
#include "World/Treeminator.hpp"

World::World(Player* player) : m_diameter(WORLD_RADIUS * 2 + 1), m_player(player) {
    std::println("Generating {} x {} chunks...", m_diameter, m_diameter);
    Timer::startTimer();
    initTexture();
    initChunks();
    generateChunkMeshAsync();
}

void World::initChunks() {
    for(int32_t chunkX = -WORLD_RADIUS; chunkX <= WORLD_RADIUS; chunkX++) {
        for(int32_t chunkZ = -WORLD_RADIUS; chunkZ <= WORLD_RADIUS; chunkZ++) {
            std::array<std::array<float, CHUNK_SIZE>, CHUNK_SIZE> heightMap;
            TerrainGenerator::generateHeightMap(heightMap, chunkX, chunkZ);
            m_chunks.emplace_back(heightMap, glm::vec3(chunkX * CHUNK_SIZE, 0.0f, chunkZ * CHUNK_SIZE));
        }
    }
}

void World::generateChunkMeshAsync() {
    for(int32_t chunkX = 0; chunkX < m_diameter; chunkX++) {
        for(int32_t chunkZ = 0; chunkZ < m_diameter; chunkZ++) {
            m_chunkThreads.enqueue([chunkX, chunkZ, this]() {
                Chunk* currentChunk = getChunk(chunkX, chunkZ);

                currentChunk->setNeighbours(getChunk(chunkX, chunkZ + 1), getChunk(chunkX, chunkZ - 1),
                getChunk(chunkX + 1, chunkZ), getChunk(chunkX - 1, chunkZ));
                Treeminator::spawnTrees(*currentChunk);

                MeshData opaqueMesh = ChunkMesh::buildOpaque(*currentChunk);
                MeshData transparentMesh = ChunkMesh::buildTransparent(*currentChunk);

                ChunkManager::updateMesh(*currentChunk, opaqueMesh, OPAQUE);
                ChunkManager::updateMesh(*currentChunk, transparentMesh, TRANSPARENT);
            });
        }
    }
}

void World::initTexture() {
    m_texture = std::make_unique<Texture>(GL_TEXTURE_2D, 1);
    m_texture->bind(0);
    m_texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    m_texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_texture->setParameter(GL_TEXTURE_MAX_LEVEL, 3);
    m_texture->loadImage("res/atlas.png");
}

World::~World() {
    for(auto& chunk : m_chunks)
        ChunkManager::deleteMesh(chunk);
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


void World::drawChunk(int32_t index, MeshType type, Shader* shader) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_chunks[index].getPosition());
    shader->setMat4("model", model);

    ChunkManager::render(m_chunks[index], type);
}


void World::render(bool wireFrameMode, Shader* worldShader, Shader* lineShader) {
    lineShader->use();
    lineShader->setVec3("color", glm::vec3(0.2f, 0.5f, 0.5f));

    Shader* currentShader = (wireFrameMode) ? lineShader : worldShader;
    currentShader->use();
    m_texture->bind(0);

    static bool isInitialized = false;
    if(!isInitialized) {
        m_chunkThreads.wait();
        m_player->setSpawn(this);

        for(auto& chunk : m_chunks) {
            ChunkManager::uploadMesh(chunk, OPAQUE);
            ChunkManager::uploadMesh(chunk, TRANSPARENT);
        }

        std::println("The chunks has finished generating...");
        std::println("Time spent: {} ms", Timer::stopTimer());
        isInitialized = true;
    }

    const ChunkCoord& chunkCoord = m_player->getChunkCoord();
    int32_t mid = getIndex(chunkCoord.chunkX, chunkCoord.chunkZ);

    for(uint8_t type = OPAQUE; type < TOTAL_MESHES; ++type) {
        int32_t i = 0;
        int32_t j = m_chunks.size() - 1;

        while(i >= 0 && i < mid || j > mid && j < m_chunks.size()) {
            if(i < mid)
                drawChunk(i++, static_cast<MeshType>(type), currentShader);
            if(j > mid)
                drawChunk(j--, static_cast<MeshType>(type), currentShader);
        }

        if(mid >= 0 && mid < m_chunks.size())
            drawChunk(mid, static_cast<MeshType>(type), currentShader);
    }
}
