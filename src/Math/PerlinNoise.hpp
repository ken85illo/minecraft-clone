#pragma once

#include <glm/glm.hpp>
#include <random>
#include <vector>

class PerlinNoise {
public:
    PerlinNoise(float frequency, float amplitude, uint16_t permutationSize, uint8_t numberOfOctaves);

    // Main function
    float fractalBrownianMotion(float x, float y);

private:
    const float m_frequency;
    const float m_amplitude;
    const uint16_t m_permutationSize;
    const uint8_t m_numberOfOctaves;

    // Helper functions
    float perlin2D(float x, float y);
    float lerp(float t, float a1, float a2);
    float fade(float t);
    glm::vec2 getConstantVec(int32_t val);
    std::vector<int32_t> makePermutation();
    void shufflePermutation(std::vector<int32_t> &permutation);
};
