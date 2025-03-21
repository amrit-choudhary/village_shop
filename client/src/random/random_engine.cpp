#include "random_engine.h"

#include <cstring>

// Random

VG::Random::Random(uint32_t seed) {
    s[0] = seed;
    s[1] = seed ^ 0x5a827999;
    s[2] = seed ^ 0x6ed9eba1;
    s[3] = seed ^ 0x8f1bbcdc;

    // Warm up the generator
    for (int i = 0; i < 10; i++) {
        Next();
    }
}

uint32_t VG::Random::Next() {
    const uint32_t result = Rotl(s[1] * 5, 7) * 9;

    const uint32_t t = s[1] << 9;

    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];

    s[2] ^= t;

    s[3] = Rotl(s[3], 11);

    return result;
}

uint32_t VG::Random::NextRange(uint32_t min, uint32_t max) {
    uint32_t range = max - min + 1;
    return min + (Next() % range);
}

double VG::Random::NextDouble() {
    return (double)Next() / 4294967296.0;  // 2^32
}

uint64_t VG::Random::Next64() {
    return ((uint64_t)Next() << 32) | Next();  // Combines two 32 bit.
}

// RandomWt

VG::RandomWt::RandomWt(uint32_t seed, uint8_t *lutValues) {
    lut = new uint8_t[10];
    std::memcpy(lut, lutValues, 10);
}

VG::RandomWt::~RandomWt() { delete[] lut; }