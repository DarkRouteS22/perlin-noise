#include "prng.hpp"
#include <chrono>
#include <iostream>

using rng = RandomNumberGenerator;

rng::RandomNumberGenerator() : RandomNumberGenerator(0) {}

rng::RandomNumberGenerator(uint64_t seed) {
    if (seed == 0) {
        state = std::chrono
        ::duration_cast<std::chrono::nanoseconds>
        (std::chrono::system_clock::now()
        .time_since_epoch()).count();
    } else {
        state = seed;
    }
    std::cout << state << std::endl;
}

uint64_t rng::l_ulong(uint64_t l_state) {
    l_state = (l_state * a + c);

    unsigned char k = (l_state >> 59) + 1; 

    l_state = (l_state << k) | (l_state >> (64 - k));

    l_state ^= l_state << 5; 
    l_state ^= l_state >> 13;
    l_state ^= l_state << 28;

    l_state *= 11745563291262604393ULL;

    k = (l_state >> 59) + 1;
    l_state = (l_state << k) | (l_state >> (64 - k)); 
    
    return l_state;
}

float rng::l_float(uint64_t l_state) {
    uint64_t x = rng::l_ulong(l_state);
    return (x >> 40) * (1.0f / (1ULL << 24));
}

uint64_t rng::next_ulong() {
    state = (state * a + c);

    unsigned char k = (state >> 59) + 1; 

    state = (state << k) | (state >> (64 - k));

    state ^= state << 5; 
    state ^= state >> 13;
    state ^= state << 28;

    state *= 11745563291262604393ULL;

    k = (state >> 59) + 1;
    state = (state << k) | (state >> (64 - k)); 
    
    return state;
}

uint64_t rng::next_ulong(uint64_t max) {
    return (next_ulong() >> 32) % max;
}

float rng::next_float() {
    uint64_t x = rng::next_ulong();
    return (x >> 40) * (1.0f / (1ULL << 24));
}