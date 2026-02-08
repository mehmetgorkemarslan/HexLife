#include "BiomeHandler.h"
#include "Config.h"

namespace mapGen {
    BiomeType BiomeHandler::getBiome(float temperature, float moisture) {
        if (temperature < Config::get<float>("biome", "max_polar_temp")) {
            return BiomeType::POLAR;
        }

        if (temperature < Config::get<float>("biome", "max_warm_temp")) {
            if (moisture < Config::get<float>("biome", "max_plain_moist")) {
                return BiomeType::PLAINS;
            }
            return BiomeType::FOREST;
        }

        if (moisture < Config::get<float>("biome", "max_desert_moist")) {
            return BiomeType::DESERT;
        }
        return BiomeType::JUNGLE;
    }
}
