#include "SimulationWorld.h"
#include "Logger.h"
#include "BiomeHandler.h"
#include "WorldManager.h"
#include <iostream>

namespace eco {

void SimulationWorld::initialize(int grid_radius, int population_size,
                                 int num_inputs, int num_outputs) {
    Logger::Log(LogLevel::INFO, "Initializing SimulationWorld...");

    // Initialize grid
    grid.generateWorld(grid_radius);
    Logger::Log(LogLevel::INFO, "Hex grid generated with radius " + std::to_string(grid_radius));

    // Initialize biomes on the grid
    mapGen::BiomeHandler::loadBiomes();
    mapGen::WorldManager worldManager(&grid);
    worldManager.addBiomes();
    Logger::Log(LogLevel::INFO, "Biomes loaded and applied to grid");

    // Initialize population
    population.initialize(population_size, grid, num_inputs, num_outputs);
    Logger::Log(LogLevel::INFO, "Population initialized with " + std::to_string(population_size) + " entities");

    elapsed_time = 0.0f;
    frame_count = 0;
    last_stat_print_time = 0.0f;
}

void SimulationWorld::run(float simulation_time, float delta_time) {
    Logger::Log(LogLevel::INFO, "Starting simulation for " + std::to_string(simulation_time) + " seconds");

    elapsed_time = 0.0f;
    frame_count = 0;

    while (elapsed_time < simulation_time) {
        step(delta_time);
        elapsed_time += delta_time;

        // Print statistics periodically
        if (elapsed_time - last_stat_print_time >= stat_print_interval) {
            print_statistics();
            last_stat_print_time = elapsed_time;
        }
    }

    Logger::Log(LogLevel::INFO, "Simulation complete");
    print_statistics();
}

void SimulationWorld::step(float delta_time) {
    // Update population (this is where all the entity logic happens)
    population.update(delta_time, grid, elapsed_time);
    frame_count++;
}

void SimulationWorld::print_statistics() const {
    std::string stats = population.get_statistics() +
                       " | Time: " + std::to_string(elapsed_time) + "s" +
                       " | Frames: " + std::to_string(frame_count);
    Logger::Log(LogLevel::INFO, stats);
    std::cout << "  " << stats << "\n";
}

}

