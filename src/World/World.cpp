#include "World.hpp"
#include "Player/Player.hpp"
#include "Terrain.hpp"
#include "Utils/Timer.hpp"

std::unique_ptr<World> World::s_instance = nullptr;

World::World() : m_diameter(WORLD_RADIUS * 2 + 1) {
    std::println("Generating {} x {} chunks...", m_diameter, m_diameter);
    Timer::startTimer();
    initTexture();

    for(int32_t chunkX = -WORLD_RADIUS; chunkX <= WORLD_RADIUS; chunkX++)
        for(int32_t chunkZ = -WORLD_RADIUS; chunkZ <= WORLD_RADIUS; chunkZ++)
            initChunk(chunkX, chunkZ, m_chunks.end());

    for(int32_t chunkX = 0; chunkX < m_diameter; chunkX++)
        for(int32_t chunkZ = 0; chunkZ < m_diameter; chunkZ++)
            generateChunkMeshAsync(chunkX, chunkZ);
}

World* World::get() {
    if(!s_instance)
        s_instance = std::make_unique<World>();

    return s_instance.get();
}

void World::initChunk(int32_t chunkX, int32_t chunkZ, std::vector<std::unique_ptr<Chunk>>::iterator it) {
    std::array<std::array<float, CHUNK_SIZE>, CHUNK_SIZE> heightMap;
    Terrain::generateHeightMap(heightMap, chunkX, chunkZ);
    m_chunks.emplace(it, std::make_unique<Chunk>(heightMap, glm::vec3(chunkX * CHUNK_SIZE, 0.0f, chunkZ * CHUNK_SIZE)));
}

Chunk* World::generateChunkMeshAsync(int32_t chunkX, int32_t chunkZ) {
    Chunk* currentChunk = getChunk(chunkX, chunkZ);
    m_chunkThreads.enqueue([currentChunk, chunkX, chunkZ, this]() {
        auto north = getChunk(chunkX, chunkZ + 1);
        auto south = getChunk(chunkX, chunkZ - 1);
        auto east = getChunk(chunkX + 1, chunkZ);
        auto west = getChunk(chunkX - 1, chunkZ);

        currentChunk->setNeighbours(north, south, east, west);

        MeshData opaqueMesh = ChunkMesh::buildOpaque(*currentChunk);
        MeshData transparentMesh = ChunkMesh::buildTransparent(*currentChunk);

        ChunkManager::updateMesh(*currentChunk, opaqueMesh, MeshType::OPAQUE);
        ChunkManager::updateMesh(*currentChunk, transparentMesh, MeshType::TRANSPARENT);
    });
    return currentChunk;
}

void World::generateChunkRight() {
    // Erase the chunks to the west
    m_chunks.erase(m_chunks.begin(), m_chunks.begin() + m_diameter);

    // Add chunks to the east
    std::queue<Chunk*> chunks;
    int32_t inputX = WORLD_RADIUS + ++m_offset.x;
    for(int32_t chunkZ = -WORLD_RADIUS; chunkZ <= WORLD_RADIUS; ++chunkZ)
        initChunk(inputX, chunkZ + m_offset.z, m_chunks.end());


    for(int32_t chunkZ = 0; chunkZ < m_diameter; ++chunkZ) {
        chunks.push(generateChunkMeshAsync(m_diameter - 1, chunkZ));
        chunks.push(generateChunkMeshAsync(m_diameter - 2, chunkZ));
    }

    m_chunkThreads.wait();
    while(!chunks.empty()) {
        ChunkManager::uploadMesh(*chunks.front(), MeshType::OPAQUE);
        ChunkManager::uploadMesh(*chunks.front(), MeshType::TRANSPARENT);
        chunks.pop();
    }
}

void World::generateChunkLeft() {
    // Erase the chunks to the east
    m_chunks.erase(m_chunks.end() - m_diameter, m_chunks.end());

    // Add chunks to the west
    std::queue<Chunk*> chunks;
    int32_t inputX = -WORLD_RADIUS + --m_offset.x;
    for(int32_t chunkZ = WORLD_RADIUS; chunkZ >= -WORLD_RADIUS; --chunkZ)
        initChunk(inputX, chunkZ + m_offset.z, m_chunks.begin());

    for(int32_t chunkZ = 0; chunkZ < m_diameter; ++chunkZ) {
        chunks.push(generateChunkMeshAsync(0, chunkZ));
        chunks.push(generateChunkMeshAsync(1, chunkZ));
    }

    m_chunkThreads.wait();
    while(!chunks.empty()) {
        ChunkManager::uploadMesh(*chunks.front(), MeshType::OPAQUE);
        ChunkManager::uploadMesh(*chunks.front(), MeshType::TRANSPARENT);
        chunks.pop();
    }
}


void World::generateChunkFront() {
    // Erase the chunks to the south
    for(int32_t chunkX = m_diameter - 1; chunkX >= 0; --chunkX)
        m_chunks.erase(m_chunks.begin() + getIndex(chunkX, 0));


    // Add chunks to the north
    std::queue<Chunk*> chunks;
    int32_t inputZ = WORLD_RADIUS + ++m_offset.z;
    for(int32_t chunkX = -WORLD_RADIUS; chunkX <= WORLD_RADIUS; ++chunkX)
        initChunk(chunkX + m_offset.x, inputZ, m_chunks.begin() + getIndex(chunkX + WORLD_RADIUS, m_diameter - 1));


    for(int32_t chunkX = 0; chunkX < m_diameter; ++chunkX) {
        chunks.push(generateChunkMeshAsync(chunkX, m_diameter - 1));
        chunks.push(generateChunkMeshAsync(chunkX, m_diameter - 2));
    }

    m_chunkThreads.wait();
    while(!chunks.empty()) {
        ChunkManager::uploadMesh(*chunks.front(), MeshType::OPAQUE);
        ChunkManager::uploadMesh(*chunks.front(), MeshType::TRANSPARENT);
        chunks.pop();
    }
}

void World::generateChunkBack() {
    // Erase the chunks to the north
    for(int32_t chunkX = m_diameter - 1; chunkX >= 0; --chunkX)
        m_chunks.erase(m_chunks.begin() + getIndex(chunkX, m_diameter - 1));


    // Add chunks to the north
    std::queue<Chunk*> chunks;
    int32_t inputZ = -WORLD_RADIUS + --m_offset.z;
    for(int32_t chunkX = -WORLD_RADIUS; chunkX <= WORLD_RADIUS; ++chunkX)
        initChunk(chunkX + m_offset.x, inputZ, m_chunks.begin() + getIndex(chunkX + WORLD_RADIUS, 0));


    for(int32_t chunkX = 0; chunkX < m_diameter; ++chunkX) {
        chunks.push(generateChunkMeshAsync(chunkX, 0));
        chunks.push(generateChunkMeshAsync(chunkX, 1));
    }

    m_chunkThreads.wait();
    while(!chunks.empty()) {
        ChunkManager::uploadMesh(*chunks.front(), MeshType::OPAQUE);
        ChunkManager::uploadMesh(*chunks.front(), MeshType::TRANSPARENT);
        chunks.pop();
    }
}

void World::sortChunks() {
    m_sortedChunks.clear();
    for(auto& chunk : m_chunks) {
        float distance = glm::length(Player::get()->getPosition() - (chunk->getPosition() + CHUNK_SIZE / 2.0f));
        m_sortedChunks.emplace(distance, chunk.get());
    }
}

void World::sortChunkFaces(int32_t chunkX, int32_t chunkZ, uint8_t radius) {
    std::queue<Chunk*> chunks;
    for(int32_t x = chunkX - radius; x <= chunkX + radius; ++x)
        for(int32_t z = chunkZ - radius; z <= chunkZ + radius; ++z) {

            auto chunk = getChunk(x, z);

            if(!chunk)
                continue;

            chunks.push(chunk);
            m_chunkThreads.enqueue([chunk, &chunks, this] {
                MeshData transparentMesh = ChunkMesh::buildTransparent(*chunk);
                ChunkManager::updateMesh(*chunk, transparentMesh, MeshType::TRANSPARENT);
            });
        }

    m_chunkThreads.wait();
    while(!chunks.empty()) {
        ChunkManager::uploadMesh(*chunks.front(), MeshType::TRANSPARENT);
        chunks.pop();
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
    return m_chunks[index].get();
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

        for(auto& chunk : m_chunks) {
            ChunkManager::uploadMesh(*chunk, MeshType::OPAQUE);
            ChunkManager::uploadMesh(*chunk, MeshType::TRANSPARENT);
        }

        std::println("The chunks has finished generating...");
        std::println("Time spent: {} ms", Timer::stopTimer());
        isInitialized = true;
    }


    for(uint8_t type = 0; type < static_cast<uint8_t>(MeshType::TOTAL_MESHES); ++type) {
        for(auto i = m_sortedChunks.rbegin(); i != m_sortedChunks.rend(); ++i) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, i->second->getPosition());
            currentShader->setMat4("model", model);

            ChunkManager::render(*i->second, static_cast<MeshType>(type));
        }
    }
}
