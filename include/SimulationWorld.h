#pragma once

#include "../eco/PopulationManager.h"
#include "MapSystem/HexGrid.h"
#include <memory>

namespace eco {
    /**
     * @brief Main orchestrator for the HexLife simulation.
     *
     * SimulationWorld manages:
     * - The hex grid (environment)
     * - The population manager (entities)
     * - The simulation loop (time stepping)
     * - Statistics gathering and reporting
     *
     * This is the top-level class that ties together all simulation components.
     */
    class SimulationWorld {
    public:
        SimulationWorld() = default;
        ~SimulationWorld() = default;

        /**
         * @brief Initialize the simulation world.
         * @param grid_radius Size of the hex grid (in hex steps from origin)
         * @param population_size Number of initial entities
         * @param num_inputs NEAT network input nodes
         * @param num_outputs NEAT network output nodes
         */
        void initialize(int grid_radius, int population_size,
                       int num_inputs, int num_outputs);

        /**
         * @brief Run the simulation for a specified duration.
         * @param simulation_time Total simulation time in seconds
         * @param delta_time Time step per frame (in seconds)
         */
        void run(float simulation_time, float delta_time = 0.01f);

        /**
         * @brief Step the simulation by one frame.
         * @param delta_time Time step (in seconds)
         */
        void step(float delta_time);

        /**
         * @brief Get the hex grid.
         * @return Reference to the HexGrid
         */
        mapGen::HexGrid& get_grid() { return grid; }
        const mapGen::HexGrid& get_grid() const { return grid; }

        /**
         * @brief Get the population manager.
         * @return Reference to the PopulationManager
         */
        PopulationManager& get_population() { return population; }
        const PopulationManager& get_population() const { return population; }

        float get_elapsed_time() const { return elapsed_time; }
        int get_frame_count() const { return frame_count; }

        /**
         * @brief Print current simulation statistics.
         */
        void print_statistics() const;

    private:
        mapGen::HexGrid grid;
        PopulationManager population;

        float elapsed_time = 0.0f;
        int frame_count = 0;

        float stat_print_interval = 1.0f;  ///< Print stats every N seconds
        float last_stat_print_time = 0.0f;
    };
}

