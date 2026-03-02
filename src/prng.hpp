#pragma once
#include <cstdint>

class RandomNumberGenerator {
private:
    uint64_t state;
    const uint64_t a = 14122281944726847361ULL, c = 717812865365;
public:
    RandomNumberGenerator();
    RandomNumberGenerator(uint64_t seed);
    uint64_t next_ulong();
    uint64_t next_ulong(uint64_t max);
    uint64_t l_ulong(uint64_t l_state);
    float next_float();
    float l_float(uint64_t l_state);
};