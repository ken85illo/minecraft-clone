#include "World.hpp"
#include "Player/Player.hpp"
#include "Terrain.hpp"
#include "Utils/Timer.hpp"

std::unique_ptr<World> World::s_instance = nullptr;

World::World() {
    // Initialize terrain seed
    Terrain::init();

    std::println("Generating {} x {} chunks...", m_diameter, m_diameter);
    Timer::startTimer();
    initTexture();

    for (int32_t x = 0; x <= m_diameter - 1; x++) {
        for (int32_t z = 0; z <= m_diameter - 1; z++) {
            initChunk(x, z);
        }
    }

    m_chunkThreads.wait();

    for (int32_t x = 0; x < m_diameter - 1; x++) {
        for (int32_t z = 0; z < m_diameter - 1; z++) {
            generateChunkMeshAsync(x, z);
        }
    }
}

World::~World() {
    for (int32_t x = 0; x <= m_diameter - 1; x++) {
        for (int32_t z = 0; z <= m_diameter - 1; z++) {
            int32_t chunkX = x - WORLD_RADIUS + m_offset.x;
            int32_t chunkZ = z - WORLD_RADIUS + m_offset.z;

            ChunkManager::serialize(*m_chunks[x][z], chunkX, chunkZ);
        }
    }
}

World *World::get() {
    if (!s_instance) {
        s_instance = std::make_unique<World>();
    }

    return s_instance.get();
}

void World::initChunk(int32_t indexX, int32_t indexZ) {
    m_chunkThreads.enqueue([=, this] {
        int32_t chunkX = indexX - WORLD_RADIUS + m_offset.x;
        int32_t chunkZ = indexZ - WORLD_RADIUS + m_offset.z;

        std::array<std::array<float, CHUNK_SIZE>, CHUNK_SIZE> heightMap;
        Terrain::generateHeightMap(heightMap, chunkX, chunkZ);

        if (ChunkManager::binaryExists(chunkX, chunkZ)) {
            m_chunks[indexX][indexZ].reset(ChunkManager::deserialize(chunkX, chunkZ));
            return;
        }

        m_chunks[indexX][indexZ] =
            std::make_unique<Chunk>(heightMap, glm::vec3(chunkX * CHUNK_SIZE, 0.0f, chunkZ * CHUNK_SIZE));
    });
}

Chunk *World::generateChunkMeshAsync(int32_t indexX, int32_t indexZ) {
    Chunk *currentChunk = m_chunks[indexX][indexZ].get();
    m_chunkThreads.enqueue([=, this]() {
        auto north = getChunk(indexX, indexZ + 1);
        auto south = getChunk(indexX, indexZ - 1);
        auto east = getChunk(indexX + 1, indexZ);
        auto west = getChunk(indexX - 1, indexZ);

        currentChunk->setNeighbours(north, south, east, west);

        MeshData opaqueMesh = ChunkMesh::buildOpaque(*currentChunk);
        MeshData transparentMesh = ChunkMesh::buildTransparent(*currentChunk);

        ChunkManager::updateMesh(*currentChunk, opaqueMesh, MeshType::OPAQUE);
        ChunkManager::updateMesh(*currentChunk, transparentMesh, MeshType::TRANSPARENT);
    });
    return currentChunk;
}

void World::generateChunkRight() {
    for (int32_t x = 0; x < m_diameter - 1; ++x) {
        for (int32_t z = 0; z < m_diameter; ++z) {
            if (x == 0) {
                int32_t chunkX = x - WORLD_RADIUS + m_offset.x;
                int32_t chunkZ = z - WORLD_RADIUS + m_offset.z;

                ChunkManager::serialize(*m_chunks[x][z], chunkX, chunkZ);
            }
            m_chunks[x][z] = std::move(m_chunks[x + 1][z]);
        }
    }

    ++m_offset.x;
    for (int32_t z = 0; z < m_diameter; ++z) {
        initChunk(m_diameter - 1, z);
    }

    m_chunkThreads.wait();

    for (int32_t z = 0; z < m_diameter; ++z) {
        generateChunkMeshAsync(m_diameter - 1, z);
        generateChunkMeshAsync(m_diameter - 2, z);
    }

    m_chunkThreads.wait();

    for (int32_t z = 0; z < m_diameter; ++z) {
        ChunkManager::uploadMesh(*m_chunks[m_diameter - 1][z], MeshType::OPAQUE);
        ChunkManager::uploadMesh(*m_chunks[m_diameter - 1][z], MeshType::TRANSPARENT);
        ChunkManager::uploadMesh(*m_chunks[m_diameter - 2][z], MeshType::OPAQUE);
        ChunkManager::uploadMesh(*m_chunks[m_diameter - 2][z], MeshType::TRANSPARENT);
    }
    sortChunks();
}

void World::generateChunkLeft() {
    for (int32_t x = m_diameter - 1; x > 0; --x) {
        for (int32_t z = 0; z < m_diameter; ++z) {
            if (x == m_diameter - 1) {
                int32_t chunkX = x - WORLD_RADIUS + m_offset.x;
                int32_t chunkZ = z - WORLD_RADIUS + m_offset.z;

                ChunkManager::serialize(*m_chunks[x][z], chunkX, chunkZ);
            }
            m_chunks[x][z] = std::move(m_chunks[x - 1][z]);
        }
    }

    --m_offset.x;
    for (int32_t z = 0; z < m_diameter; ++z) {
        initChunk(0, z);
    }

    m_chunkThreads.wait();

    for (int32_t z = 0; z < m_diameter; ++z) {
        generateChunkMeshAsync(0, z);
        generateChunkMeshAsync(1, z);
    }

    m_chunkThreads.wait();

    for (int32_t z = 0; z < m_diameter; ++z) {
        ChunkManager::uploadMesh(*m_chunks[0][z], MeshType::OPAQUE);
        ChunkManager::uploadMesh(*m_chunks[0][z], MeshType::TRANSPARENT);
        ChunkManager::uploadMesh(*m_chunks[1][z], MeshType::OPAQUE);
        ChunkManager::uploadMesh(*m_chunks[1][z], MeshType::TRANSPARENT);
    }
    sortChunks();
}

void World::generateChunkFront() {
    for (int32_t z = 0; z < m_diameter - 1; ++z) {
        for (int32_t x = 0; x < m_diameter; ++x) {
            if (z == 0) {
                int32_t chunkX = x - WORLD_RADIUS + m_offset.x;
                int32_t chunkZ = z - WORLD_RADIUS + m_offset.z;

                ChunkManager::serialize(*m_chunks[x][z], chunkX, chunkZ);
            }
            m_chunks[x][z] = std::move(m_chunks[x][z + 1]);
        }
    }

    ++m_offset.z;
    for (int32_t x = 0; x < m_diameter; ++x) {
        initChunk(x, m_diameter - 1);
    }

    m_chunkThreads.wait();

    for (int32_t x = 0; x < m_diameter; ++x) {
        generateChunkMeshAsync(x, m_diameter - 1);
        generateChunkMeshAsync(x, m_diameter - 2);
    }

    m_chunkThreads.wait();

    for (int32_t x = 0; x < m_diameter; ++x) {
        ChunkManager::uploadMesh(*m_chunks[x][m_diameter - 1], MeshType::OPAQUE);
        ChunkManager::uploadMesh(*m_chunks[x][m_diameter - 1], MeshType::TRANSPARENT);
        ChunkManager::uploadMesh(*m_chunks[x][m_diameter - 2], MeshType::OPAQUE);
        ChunkManager::uploadMesh(*m_chunks[x][m_diameter - 2], MeshType::TRANSPARENT);
    }

    sortChunks();
}

void World::generateChunkBack() {
    for (int32_t z = m_diameter - 1; z > 0; --z) {
        for (int32_t x = 0; x < m_diameter; ++x) {
            if (z == m_diameter - 1) {
                int32_t chunkX = x - WORLD_RADIUS + m_offset.x;
                int32_t chunkZ = z - WORLD_RADIUS + m_offset.z;

                ChunkManager::serialize(*m_chunks[x][z], chunkX, chunkZ);
            }
            m_chunks[x][z] = std::move(m_chunks[x][z - 1]);
        }
    }

    --m_offset.z;
    for (int32_t x = 0; x < m_diameter; ++x) {
        initChunk(x, 0);
    }

    m_chunkThreads.wait();

    for (int32_t x = 0; x < m_diameter; ++x) {
        generateChunkMeshAsync(x, 0);
        generateChunkMeshAsync(x, 1);
    }

    m_chunkThreads.wait();

    for (int32_t x = 0; x < m_diameter; ++x) {
        ChunkManager::uploadMesh(*m_chunks[x][0], MeshType::OPAQUE);
        ChunkManager::uploadMesh(*m_chunks[x][0], MeshType::TRANSPARENT);
        ChunkManager::uploadMesh(*m_chunks[x][1], MeshType::OPAQUE);
        ChunkManager::uploadMesh(*m_chunks[x][1], MeshType::TRANSPARENT);
    }

    sortChunks();
}

void World::sortChunks() {
    m_sortedChunks.clear();

    for (int32_t x = 0; x < m_diameter; ++x) {
        for (int32_t z = 0; z < m_diameter; ++z) {
            float distance =
                glm::length(Player::get()->getPosition() - (m_chunks[x][z]->getPosition() + CHUNK_SIZE / 2.0f));
            m_sortedChunks.emplace(distance, m_chunks[x][z].get());
        }
    }
}

void World::sortChunkFaces(int32_t chunkX, int32_t chunkZ, uint8_t radius) {
    std::queue<Chunk *> chunks;
    for (int32_t x = chunkX - radius; x <= chunkX + radius; ++x) {
        for (int32_t z = chunkZ - radius; z <= chunkZ + radius; ++z) {
            auto chunk = getChunk(x, z);

            if (!chunk) {
                continue;
            }

            chunks.push(chunk);
            m_chunkThreads.enqueue([chunk, &chunks, this] {
                MeshData transparentMesh = ChunkMesh::buildTransparent(*chunk);
                ChunkManager::updateMesh(*chunk, transparentMesh, MeshType::TRANSPARENT);
            });
        }
    }

    m_chunkThreads.wait();
    while (!chunks.empty()) {
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

Chunk *World::getChunk(int32_t x, int32_t z) {
    if (x < 0 || x >= m_diameter || z < 0 || z >= m_diameter) {
        return nullptr;
    }

    return m_chunks[x][z].get();
}

void World::render(bool wireFrameMode, Shader *worldShader, Shader *lineShader) {
    lineShader->use();
    lineShader->setVec3("color", glm::vec3(0.2f, 0.5f, 0.5f));

    Shader *currentShader = (wireFrameMode) ? lineShader : worldShader;
    currentShader->use();
    m_texture->bind(0);

    static bool isInitialized = false;
    if (!isInitialized) {
        m_chunkThreads.wait();

        for (int32_t x = 0; x < m_diameter; ++x) {
            for (int32_t z = 0; z < m_diameter; ++z) {
                ChunkManager::uploadMesh(*m_chunks[x][z], MeshType::OPAQUE);
                ChunkManager::uploadMesh(*m_chunks[x][z], MeshType::TRANSPARENT);
            }
        }

        std::println("The chunks has finished generating...");
        std::println("Time spent: {} ms", Timer::stopTimer());
        isInitialized = true;
    }

    for (uint8_t type = 0; type < static_cast<uint8_t>(MeshType::TOTAL_MESHES); ++type) {
        for (auto i = m_sortedChunks.rbegin(); i != m_sortedChunks.rend(); ++i) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, i->second->getPosition());
            currentShader->setMat4("model", model);

            ChunkManager::render(*i->second, static_cast<MeshType>(type));
        }
    }
}
