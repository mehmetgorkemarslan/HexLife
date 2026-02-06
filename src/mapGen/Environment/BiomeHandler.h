#pragma once
#include "Common.h"

namespace mapGen {
    class BiomeHandler {
    public:
        static BiomeType getBiome(float temperature, float moisture);
    };
}
