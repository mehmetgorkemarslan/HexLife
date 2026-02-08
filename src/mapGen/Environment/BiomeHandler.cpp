#include "BiomeHandler.h"

namespace mapGen {
    void BiomeHandler::loadBiomes() {
        // Clear one before. Just in case
        getInstance().biomes.clear();
        getInstance().nameToId.clear();

        auto biomesConfig = Config::getSection("biome", "types");

        BiomeID currentID = 0;

        for (auto &[key, value]: biomesConfig.items()) {
            Color c = getColor(value);
            BiomeStats bs = getBiomeStats(value);

            Biome newBiome(currentID, key, c, bs);
            getInstance().biomes.push_back(newBiome);
            getInstance().nameToId[key] = currentID;
            currentID++;
        }
    }

    BiomeID BiomeHandler::calculateBiomeID(float temperature, float moisture) {
        // Cache for performance
        static BiomeID polarID = getBiomeIdByName("polar");
        static BiomeID plainsID = getBiomeIdByName("plains");
        static BiomeID forestID = getBiomeIdByName("forest");
        static BiomeID desertID = getBiomeIdByName("desert");
        static BiomeID jungleID = getBiomeIdByName("jungle");

        static float tempPolar = Config::get<float>("biome", "thresholds", "max_polar_temp");
        static float tempWarm = Config::get<float>("biome", "thresholds", "max_warm_temp");
        static float moistPlain = Config::get<float>("biome", "thresholds", "max_plain_moist");
        static float moistDesert = Config::get<float>("biome", "thresholds", "max_desert_moist");

        if (temperature < tempPolar) {
            return polarID;
        }

        if (temperature < tempWarm) {
            if (moisture < moistPlain) {
                return plainsID;
            }
            return forestID;
        }

        if (moisture < moistDesert) {
            return desertID;
        }
        return jungleID;
    }
}