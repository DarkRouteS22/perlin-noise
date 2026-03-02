#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "prng.hpp"

struct Vec2 {
    float x, y;
};

RandomNumberGenerator rng(123456789);

const uint64_t SEED = rng.next_ulong();

const int SIZE = 800;
const int SECTOR_SIZE = 200;
const int GRADS_SIZE = (SIZE / SECTOR_SIZE) + 1;

std::vector<unsigned char> pixels(SIZE * SIZE);

Vec2 getGrad(int x, int y) {
    uint64_t hash = ((uint64_t)x * 374761393 + (uint64_t)y * 668265263) ^ SEED;
    float angle = rng.l_float(hash) * 2.0f * 3.14159265f;
    return { std::cos(angle), std::sin(angle) };
}

float fade(float t) {
    return ((6*t - 15)*t + 10)*t*t*t;  
}

float dot(Vec2 a, Vec2 b) {
    return a.x * b.x + a.y * b.y;
}

float perlin(float x, float y, float frequency) {
    x *= frequency;
    y *= frequency;
    int x0 = (int)std::floor(x);
    int y0 = (int)std::floor(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    Vec2 grad00 = getGrad(x0, y0);
    Vec2 grad10 = getGrad(x1, y0);
    Vec2 grad01 = getGrad(x0, y1);
    Vec2 grad11 = getGrad(x1, y1);

    float lx = x - x0;
    float ly = y - y0;

    Vec2 dist00 = {lx    , ly    };
    Vec2 dist10 = {lx - 1, ly    };
    Vec2 dist01 = {lx    , ly - 1};
    Vec2 dist11 = {lx - 1, ly - 1};

    float dot00 = dot(grad00, dist00);
    float dot01 = dot(grad01, dist01);
    float dot10 = dot(grad10, dist10);
    float dot11 = dot(grad11, dist11);
    
    float fadeX = fade(lx);
    float fadeY = fade(ly);

    float dotx0 = dot00 + fadeX * (dot10 - dot00);
    float dotx1 = dot01 + fadeX * (dot11 - dot01);

    return ((dotx0 + fadeY * (dotx1 - dotx0)) + 1.0f) * 0.5f;
}

float fbm(float x, float y) {
    float result = 0.0f;
    float maxValue = 0.0f;

    float amplitude = 0.5f;
    float frequency = 0.01f;
    float persistence = 0.5f;
    float lacunarity = 2.5f;
    int octave = 8;

    for (int i = 0; i < octave; i++) {
        result += perlin(x, y, frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= lacunarity;
    }

    float value = result / maxValue;
    return value;
}

float turbulence_fbm(float x, float y, float coeff) {
    float dx = fbm(x, y) * coeff;
    float dy = fbm(x + 1000.0f, y + 1000.0f) * coeff;

    return fbm(x + dx, y + dy);
}

int main() {
    std::cout << "Генерация шума..." << std::endl;

    for (int index = 0; index < pixels.size(); index ++) {
        Vec2 cord;
        cord.x = (index % SIZE);
        cord.y = (int)(index / SIZE);

        float value = turbulence_fbm(cord.x, cord.y, 250);
        value = std::pow(value, 0.9f);
        value = std::clamp((value - 0.4f) * 2.0f, 0.0f, 1.0f);

        pixels[index] = value * 255;
    }   

    std::ofstream out("noise.ppm", std::ios::binary);

    out << "P6\n" << SIZE * 2 << " " << SIZE << "\n255\n";

    for (int y = 0; y < SIZE; y++) {

        for (int x = 0; x < SIZE; x++) {
            int index = y * SIZE + x;
            unsigned char value = pixels[index];
            out.write((char*)&value, 1);
            out.write((char*)&value, 1);
            out.write((char*)&value, 1);
        }

        for (int x = 0; x < SIZE; x++) {
            int index = y * SIZE + x;

            float value = (float)pixels[index] / 255;

            unsigned char r, g, b;
            if (value < 0.3f) {
                r = 0;
                g = 0;
                b = (unsigned char)(180 * (value + 0.2f) * 1.5f);
            } else if (value < 0.32f) {
                r = (unsigned char)(240 * value);
                g = (unsigned char)(230 * value);
                b = (unsigned char)(140 * value);
            } else if (value < 0.45f) {
                r = (unsigned char)(20 * value);
                g = (unsigned char)(160 * value);
                b = (unsigned char)(20 * value);
            } else {
                r = (unsigned char)(200 * value);
                g = (unsigned char)(200 * value);
                b = (unsigned char)(200 * value);
            }

            out.write((char*)&r, 1);
            out.write((char*)&g, 1);
            out.write((char*)&b, 1);
        }
    }

    out.close();

    return 0;
}