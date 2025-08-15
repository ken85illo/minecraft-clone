#pragma once

#include "Chunk/Chunk.hpp"
#include "Shader/Shader.hpp"
#include "Texture/Texture.hpp"
#include "Utils/ThreadPool.hpp"
#include <cstdint>
#include <map>

#define WORLD_RADIUS 12

class World {
public:
    World(Player* player);
    ~World();

    void render(bool wireFrameMode, Shader* worldShader, Shader* lineShader);
    void sortChunks();
    void sortChunkFaces(int32_t chunkX, int32_t chunkZ, uint8_t radius);

    const int32_t getDiameter() const;
    const int32_t getIndex(int32_t x, int32_t z) const;
    Chunk* getChunk(int32_t x, int32_t z);

private:
    std::unique_ptr<Texture> m_texture;
    int32_t m_diameter;

    std::vector<Chunk> m_chunks;
    std::multimap<float, Chunk*> m_sortedChunks;
    ThreadPool m_chunkThreads;
    Player* m_player;

    void initChunks();
    void generateChunkMeshAsync();
    void initTexture();
    void drawChunk(Chunk& chunk, MeshType type, Shader* shader);
};
