#include <iostream>

#include "Config.h"
#include "Logger.h"
#include "HexGrid.h"
#include "MapVisualizer.h"
#include "PerlinNoise.h" // For perlin noise test
#include "WorldManager.h"
// TODO: Create a test class

void NoiseSystemTest() {
    Logger::Log(LogLevel::INFO, "--- Noise System Stress Test ---");
    mapGen::PerlinNoise testNoise(12345, 1, 0.5f, 2.0f);

    float centerVal = testNoise.sample(0.0f, 0.0f);
    Logger::Log(LogLevel::DEBUG, "Center Value (0,0): " + std::to_string(centerVal));

    float smallStep = testNoise.sample(0.01f, 0.01f);
    float largeStep = testNoise.sample(0.5f, 0.5f);
    float farStep = testNoise.sample(5.3f, 10.7f);

    Logger::Log(LogLevel::DEBUG, "Small Step Value (0.01): " + std::to_string(smallStep));
    Logger::Log(LogLevel::DEBUG, "large Step Value(0.50): " + std::to_string(largeStep));
    Logger::Log(LogLevel::DEBUG, "Far Step Value  (5.3, 10.7): " + std::to_string(farStep));

    // 1000 Sample
    float minV = 1.0f, maxV = 0.0f;
    for(int i = 0; i < 1000; i++) {
        float val = testNoise.sample(i * 0.123f, i * 0.456f);
        if(val < minV) minV = val;
        if(val > maxV) maxV = val;
    }

    Logger::Log(LogLevel::INFO, "Results = Min: " + std::to_string(minV) + " Max: " + std::to_string(maxV));
}

void LoggerTest() {
    Logger::Log(LogLevel::ERROR, "Error test message");
    Logger::Log(LogLevel::DEBUG, "this message is buffered");
    Logger::Log(LogLevel::INFO, "This message too");
    Logger::Log(LogLevel::FATAL, "This prints directly");
}

void MapGenTest() {
    auto *grid = new mapGen::HexGrid();
    auto* wordManager = new mapGen::WorldManager(grid);

    grid->generateWorld(Config::get<int>("map_radius"));
    wordManager->addBiomes();

    // mapGen::MapVisualizer::DisplayAllGrid(*grid);
    mapGen::MapVisualizer::exportToPNG(*grid, "Map.png");

    delete grid;
}

int main() {
    Config::load();
    Logger::Init(Config::get<std::string>("log_file_name"));

    // NoiseSystemTest();
    MapGenTest();

    // Flush Buffer at end of every loop
    Logger::FlushBuffer();

    // std::cout << "\nPress Enter to exit...";
    // std::cin.get();
    return 0;
}