#include "PerlinNoise.h"

#include <algorithm>
#include <cmath>
#include "Config.h"

namespace mapGen {
    PerlinNoise::PerlinNoise(unsigned int seed, int octaves, float persistence, float lacunarity) : seed(seed),
        octaves(octaves), persistence(persistence), lacunarity(lacunarity) {
    }

    float PerlinNoise::sample(float x, float y) const {
        float total = 0.0f;
        float freq = 1.0f;
        float amplitude = 1.0f;
        float maxValue = 0.0f;

        for (int i = 0; i < octaves; i++) {
            total += calculateSingleLayer(x * freq, y * freq) * amplitude;
            maxValue += amplitude;

            amplitude *= persistence;
            freq *= lacunarity;
        }

        return smoothstep(Config::get<float>("stretch_min"), Config::get<float>("stretch_max"),
                          (total / maxValue + 1.0f) / 2.0f);
    }

    void PerlinNoise::setFractalSettings(int octaves, float persistence, float lacunarity) {
        this->octaves = octaves;
        this->persistence = persistence;
        this->lacunarity = lacunarity;
    }

    float PerlinNoise::calculateSingleLayer(float x, float y) const {
        int x0 = static_cast<int>(std::floor(x));
        int y0 = static_cast<int>(std::floor(y));
        int x1 = x0 + 1;
        int y1 = y0 + 1;

        float sx = x - static_cast<float>(x0);
        float sy = y - static_cast<float>(y0);

        float n0 = dotGridGradient(x0, y0, x, y);
        float n1 = dotGridGradient(x1, y0, x, y);
        float ix0 = interpolate(n0, n1, sx);

        n0 = dotGridGradient(x0, y1, x, y);
        n1 = dotGridGradient(x1, y1, x, y);
        float ix1 = interpolate(n0, n1, sx);

        float value = interpolate(ix0, ix1, sy);

        return value;
    }

    Vector2 PerlinNoise::randomGradient(int ix, int iy) const {
        const unsigned w = 8 * sizeof(unsigned);
        const unsigned s = w / 2;

        // Add seed
        unsigned a = ix + seed;
        unsigned b = iy + seed;

        a *= 3284157443;
        b ^= a << s | a >> (w - s);
        b *= 1911520717;

        a ^= b << s | b >> (w - s);
        a *= 2048319325;

        float random = a * (6.2831853f / static_cast<float>(~(~0u >> 1)));

        return {std::sin(random), std::cos(random)};
    }

    float PerlinNoise::interpolate(float a0, float a1, float w) const {
        // Smooth step
        return (a1 - a0) * (3.0f - 2.0f * w) * w * w + a0;
    }

    float PerlinNoise::dotGridGradient(int ix, int iy, float x, float y) const {
        Vector2 gradient = randomGradient(ix, iy);

        float dx = x - static_cast<float>(ix);
        float dy = y - static_cast<float>(iy);

        return (dx * gradient.x + dy * gradient.y);
    }
}
