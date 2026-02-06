#pragma once
#include "Common.h"

namespace mapGen {
    /**
       @brief Abstract base type for 2D noise generators.
       @details Implementations provide a sample method returning a float noise
                value for given coordinates. Concrete implementations define
                value range semantics (e.g. [-1,1] or [0,1]).
    */
    class NoiseGenerator {
    public:
        virtual ~NoiseGenerator() = default;

        /**
           @brief Sample the noise generator at the given coordinates.
           @param x X coordinate in noise space.
           @param y Y coordinate in noise space.
           @returns Noise value; consult concrete implementation for range and meaning.
        */
        virtual float sample(float x, float y) const = 0;
    };
}
