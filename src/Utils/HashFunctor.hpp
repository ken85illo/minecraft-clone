#pragma once

#include <Chunk/ChunkContext.hpp>

struct HashFunctor {
    const size_t operator()(const ChunkCoords &c) const {
        size_t seed = 0;
        hashCombine(seed, c.x);
        hashCombine(seed, c.z);
        return seed;
    }

private:
    // Boost's hash function
    template <typename T>
    static inline void hashCombine(std::size_t &seed, const T &v) {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    };
};
