#include "WorldManager.h"

#include "BiomeHandler.h"
#include "Config.h"
#include "Logger.h"
#include "PerlinNoise.h"

namespace mapGen {
    WorldManager::WorldManager(HexGrid *grid) : targetGrid(grid) {
        auto noiseSettings = Config::getSection("noise");
        tSeed = noiseSettings.at("seed_temp").get<int>();
        mSeed = noiseSettings.at("seed_moist").get<int>();

        // Offset based on seed and random numbers
        tOffset = (float) (tSeed % 10000) * 203.921f;
        mOffset = (float) (mSeed % 10000) * 465.643f;

        int octave = noiseSettings.at("octave").get<int>();
        float persistence = noiseSettings.at("persistence").get<float>();
        float lacunarity = noiseSettings.at("lacunarity").get<float>();

        tempNoise = std::make_unique<PerlinNoise>(tSeed, octave, persistence, lacunarity);
        moistureNoise = std::make_unique<PerlinNoise>(mSeed, octave, persistence, lacunarity);
    }

    void WorldManager::addBiomes() {
        if (!targetGrid) return;

        // Scale attributes for distributions
        float tScale = Config::get<float>("noise", "scale_temp");
        float mScale = Config::get<float>("noise", "scale_moist");

        Logger::Log(LogLevel::DEBUG, "Adding biomes to world...");

        for (auto &[hex, cell]: targetGrid->getMap()) {
            Vector2 pos = Hex::toCartesian(hex);
            cell.temperature = tempNoise->sample((pos.x + tOffset) * tScale, (pos.y + tOffset) * tScale);
            cell.moisture = moistureNoise->sample((pos.x + mOffset) * mScale, (pos.y + mOffset) * mScale);
            // Get biome
            cell.biome = BiomeHandler::getBiome(cell.temperature, cell.moisture);
        }

        Logger::Log(LogLevel::DEBUG, "Adding biome done");
    }
}
