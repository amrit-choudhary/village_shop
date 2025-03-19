/**
 * Core function for random number generator.
 * This is the xoshiro128** 1.1 algorithm. It can generate 32 bit uint32 random number.
 * Reference: https://prng.di.unimi.it/xoshiro128starstar.c
 */
#pragma once

#include <cstdint>

namespace VG {

// Xoshiro128** RNG implementation.
class Random {
   private:
    uint32_t s[4];

    static inline uint32_t Rotl(const uint32_t x, int k) { return (x << k) | (x >> (32 - k)); }

   public:
    // Constructor with seed.
    Random(uint32_t seed);

    /** Generate next random number in the range of 0 to uint32_t maximum.
     * Better use some of the helper function like range.
     */
    uint32_t Next();

    // Generate a random number in range [min, max].
    uint32_t NextRange(uint32_t min, uint32_t max);

    // Generates a double in range [0, 1).
    double NextDouble();

    // Generates a uint64_t.
    uint64_t Next64();
};
}  // namespace VG