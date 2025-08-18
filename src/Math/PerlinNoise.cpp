#include "PerlinNoise.hpp"

// NOTE: This code is ported from a javascript source code I found on the internet
// See: https://rtouti.github.io/graphics/perlin-noise-algorithm

PerlinNoise::PerlinNoise(float frequency, float amplitude, uint16_t permutationSize, uint8_t numberOfOctaves)
: m_frequency(frequency),
  m_amplitude(amplitude),
  m_permutationSize(permutationSize),
  m_numberOfOctaves(numberOfOctaves) {}

float PerlinNoise::fractalBrownianMotion(float x, float y) {
    float result = 0.0f;
    float amplitude = m_amplitude;
    float frequency = m_frequency;

    for (int32_t octave = 0; octave < m_numberOfOctaves; octave++) {
        const float n = amplitude * perlin2D(x * frequency, y * frequency);
        result += n;

        amplitude *= 0.5;
        frequency *= 2.0;
    }
    return result;
}

float PerlinNoise::perlin2D(float x, float y) {
    // Wrap around when negative
    x += (x < 0) ? m_permutationSize : 0;
    y += (y < 0) ? m_permutationSize : 0;

    // same as modulo but much faster cause of bitwise
    const int32_t X = static_cast<int32_t>(x) & (m_permutationSize - 1);
    const int32_t Y = static_cast<int32_t>(y) & (m_permutationSize - 1);

    const float xf = x - floor(x);
    const float yf = y - floor(y);

    const glm::vec2 topRight = glm::vec2(xf - 1.0f, yf - 1.0f);
    const glm::vec2 topLeft = glm::vec2(xf, yf - 1.0f);
    const glm::vec2 bottomRight = glm::vec2(xf - 1.0f, yf);
    const glm::vec2 bottomLeft = glm::vec2(xf, yf);

    static std::vector<int32_t> permutation = {};

    if (permutation.empty()) {
        permutation = makePermutation();
    }

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

float PerlinNoise::lerp(float t, float a1, float a2) {
    return a1 + t * (a2 - a1);
}

float PerlinNoise::fade(float t) {
    return ((6 * t - 15) * t + 10) * t * t * t;
}

glm::vec2 PerlinNoise::getConstantVec(int32_t val) {
    val &= 3;
    switch (val) {
    case 0:
        return glm::vec2(1.0f, 1.0f);
    case 1:
        return glm::vec2(1.0f, -1.0f);
    case 2:
        return glm::vec2(-1.0f, -1.0f);
    case 3:
        return glm::vec2(-1.0f, 1.0f);
    };
    return glm::vec3(0.0f);
}

std::vector<int32_t> PerlinNoise::makePermutation() {
    std::vector<int32_t> permutation;

    for (int32_t i = 0; i < m_permutationSize; i++) {
        permutation.emplace_back(i);
    }

    shufflePermutation(permutation);

    for (int32_t i = 0; i < m_permutationSize; i++) {
        permutation.emplace_back(permutation[i]);
    }

    return permutation;
}

void PerlinNoise::shufflePermutation(std::vector<int32_t> &permutation) {
    for (int32_t i = 0; i < m_permutationSize; i++) {
        int32_t index = std::rand() % m_permutationSize;
        std::swap(permutation[i], permutation[index]);
    }
}
