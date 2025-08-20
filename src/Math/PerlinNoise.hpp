#pragma once

#include <glm/glm.hpp>
#include <random>
#include <vector>

class PerlinNoise {
public:
    PerlinNoise();
    PerlinNoise(float frequency, float amplitude, uint8_t numberOfOctaves);

    // Main function
    float fractalBrownianMotion(float x, float y);

private:
    static constexpr uint16_t m_permutationSize = 512; // Must be a power of 2 (e.g. 128, 256, 512, 1024...)
    uint16_t m_permutation[m_permutationSize * 2];

    float m_frequency;
    float m_amplitude;
    uint8_t m_numberOfOctaves;

    // Helper functions
    float perlin2D(float x, float y);
    float lerp(float t, float a1, float a2);
    float fade(float t);
    glm::vec2 getConstantVec(int32_t val);
    void makePermutation();
    void shufflePermutation();
};
