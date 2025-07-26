#include "World.hpp"

World::World(Player* player) : m_size(WORLD_RADIUS * 2 + 1) {
    for(int32_t chunkX = -WORLD_RADIUS; chunkX <= WORLD_RADIUS; chunkX++)
        for(int32_t chunkZ = -WORLD_RADIUS; chunkZ <= WORLD_RADIUS; chunkZ++) {
            float heightMap[CHUNK_SIZE][CHUNK_SIZE];
            generateHeightMap(heightMap, chunkX + WORLD_RADIUS, chunkZ + WORLD_RADIUS);

            m_chunks.emplace_back(heightMap,
            glm::vec3(chunkX * CHUNK_SIZE, 0.0f, chunkZ * CHUNK_SIZE));
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

    Engine::worldShader->use();
    Engine::worldShader->setInt("texture0", 0);
}

World::~World() {
    delete m_texture;
}

Chunk* World::getChunk(uint16_t x, uint16_t z) {
    if(x < 0 || x >= m_size || z < 0 || z >= m_size)
        return nullptr;

    uint16_t index = x * m_size + z;
    return &m_chunks[index];
}


void World::generateHeightMap(float heightMap[CHUNK_SIZE][CHUNK_SIZE], int32_t chunkX, int32_t chunkZ) {
    for(int32_t x = 0; x < CHUNK_SIZE; x++) {
        for(int32_t z = 0; z < CHUNK_SIZE; z++) {
            float n = fractalBrownianMotion(
            (chunkX * CHUNK_SIZE + x), (chunkZ * CHUNK_SIZE + z));

            n += 1.0f;
            n /= 2.0f;

            heightMap[x][z] = n;
        }
    }
}

float World::fractalBrownianMotion(float x, float y) {
    float result = 0.0f;
    float amplitude = 1.0f;
    float frequency = 0.005f;

    for(int32_t octave = 0; octave < NUMBER_OF_OCTAVES; octave++) {
        const float n = amplitude * perlin2D(x * frequency, y * frequency);
        result += n;

        amplitude *= 0.5;
        frequency *= 2.0;
    }
    return result;
}

float World::perlin2D(float x, float y) {
    // same as modulo but much faster cause of bitwise
    const int32_t X = static_cast<int32_t>(x) & (PERMUTATION_SIZE - 1);
    const int32_t Y = static_cast<int32_t>(y) & (PERMUTATION_SIZE - 1);

    const float xf = x - floor(x);
    const float yf = y - floor(y);

    const glm::vec2 topRight = glm::vec2(xf - 1.0f, yf - 1.0f);
    const glm::vec2 topLeft = glm::vec2(xf, yf - 1.0f);
    const glm::vec2 bottomRight = glm::vec2(xf - 1.0f, yf);
    const glm::vec2 bottomLeft = glm::vec2(xf, yf);

    static std::vector<int32_t> permutation = {};

    if(permutation.empty())
        permutation = makePermutation();


    const int32_t valueTopRight = permutation[permutation[X + 1] + Y + 1];
    const int32_t valueTopLeft = permutation[permutation[X] + Y + 1];
    const int32_t valueBottomRight = permutation[permutation[X + 1] + Y];
    const int32_t valueBottomLeft = permutation[permutation[X] + Y];

    const float dotTopRight = glm::dot(topRight, getConstantVec(valueTopRight));
    const float dotTopLeft = glm::dot(topLeft, getConstantVec(valueTopLeft));
    const float dotBottomRight = glm::dot(bottomRight, getConstantVec(valueBottomRight));
    const float dotBottomLeft = glm::dot(bottomLeft, getConstantVec(valueBottomLeft));

    const float u = fade(xf);
    const float v = fade(yf);

    return lerp(u, lerp(v, dotBottomLeft, dotTopLeft), lerp(v, dotBottomRight, dotTopRight));
}

float World::lerp(float t, float a1, float a2) {
    return a1 + t * (a2 - a1);
}

float World::fade(float t) {
    return ((6 * t - 15) * t + 10) * t * t * t;
}

glm::vec2 World::getConstantVec(int32_t val) {
    val &= 4; // same as modulo
    switch(val) {
    case 0: return glm::vec2(1.0f, 1.0f);
    case 1: return glm::vec2(1.0f, -1.0f);
    case 2: return glm::vec2(-1.0f, -1.0f);
    case 3: return glm::vec2(-1.0f, 1.0f);
    };
    return glm::vec3(0.0f);
}

std::vector<int32_t> World::makePermutation() {
    std::vector<int32_t> permutation;

    for(int32_t i = 0; i < PERMUTATION_SIZE; i++)
        permutation.emplace_back(i);

    shufflePermutation(permutation);

    for(int32_t i = 0; i < PERMUTATION_SIZE; i++)
        permutation.emplace_back(permutation[i]);

    return permutation;
}

void World::shufflePermutation(std::vector<int32_t>& permutation) {
    for(int32_t i = 0; i < PERMUTATION_SIZE; i++) {
        int32_t index = std::rand() % PERMUTATION_SIZE;
        std::swap(permutation[i], permutation[index]);
    }
}

void World::render() {
    Engine::worldShader->use();
    m_texture->bind(0);
    for(auto& chunk : m_chunks) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, chunk.position);
        Engine::worldShader->setMat4("model", model);

        chunk.render();
    }
}
