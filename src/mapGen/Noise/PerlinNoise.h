#pragma once
#include "NoiseGenerator.h"

namespace mapGen {
    /**
     @brief 2D Perlin noise generator implementation.
     @details Provides sampling of fractal Perlin noise with configurable seed and fractal parameters.
             Noise values returned by sample() are normalized to the [0,1] range.
    */
    class PerlinNoise : public NoiseGenerator {
    public:
        /**
         @brief Construct a PerlinNoise generator with fractal settings.
         @param seed Seed used for gradient hashing; different seeds produce different noise instances.
         @param octaves Number of fractal octaves to combine.
         @param persistence Amplitude scaling factor between successive octaves.
         @param lacunarity Frequency scaling factor between successive octaves.
        */
        explicit PerlinNoise(unsigned int seed = 0, int octaves = 1, float persistence = 0.5f, float lacunarity = 2);

        /**
         @brief Sample the Perlin noise at given 2D coordinates.
         @param x X coordinate in noise space.
         @param y Y coordinate in noise space.
         @returns Noise value normalized to the [0,1] range.
        */
        float sample(float x, float y) const override;

        /**
         @brief Configure fractal parameters used by the generator.
         @param octaves Number of octaves to use when sampling fractal noise.
         @param persistence Amplitude multiplier applied per-octave.
         @param lacunarity Frequency multiplier applied per-octave.
         @returns void
        */
        void setFractalSettings(int octaves, float persistence, float lacunarity);

    private:
        unsigned int seed;

        int octaves;
        float persistence;
        float lacunarity;

        /**
         @brief Compute a single octave (single-layer) Perlin noise value at given coordinates.
         @param x Exact X sample coordinate.
         @param y Exact Y sample coordinate.
         @returns Raw single-layer Perlin noise value (typically in approximately [-1,1]).
        */
        float calculateSingleLayer(float x, float y) const;

        /**
         @brief Generate a pseudorandom unit-length gradient vector for integer grid coordinates.
         @param ix Integer X coordinate of the grid corner.
         @param iy Integer Y coordinate of the grid corner.
         @returns Unit 2D vector representing the gradient at the provided grid point.
        */
        Vector2 randomGradient(int ix, int iy) const;

        /**
         @brief Smooth cubic interpolation between two scalar values.
         @param a0 Value at weight 0.
         @param a1 Value at weight 1.
         @param w Interpolation weight in [0,1].
         @returns Interpolated value.
        */
        float interpolate(float a0, float a1, float w) const;

        /**
         @brief Compute dot product between gradient at integer grid corner and distance vector to sample point.
         @param ix Integer X coordinate of the grid corner.
         @param iy Integer Y coordinate of the grid corner.
         @param x Exact X sample coordinate.
         @param y Exact Y sample coordinate.
         @returns Dot product of the gradient and the distance vector.
        */
        float dotGridGradient(int ix, int iy, float x, float y) const;

        static constexpr float smoothstep(float edge0, float edge1, float x) {
            float t = (x - edge0) / (edge1 - edge0);
            t = (t < 0.0f) ? 0.0f : ((t > 1.0f) ? 1.0f : t);

            return t * t * (3.0f - 2.0f * t);
        }
    };
}
