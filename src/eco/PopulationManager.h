#pragma once

#include "Entity.h"
#include "../neat/evolution/Species.h"
#include "MapSystem/HexGrid.h"
#include <vector>
#include <memory>
#include "../neat/evolution/InnovationTracker.h"

namespace eco {
    /**
     * @brief Manages all entities in the simulation, handles reproduction,
     * death, speciation, and fitness evaluation.
     *
     * This class is responsible for:
     * - Maintaining the population of entities
     * - Updating each entity's brain and behavior each frame
     * - Handling reproduction (offspring creation and genome mutation)
     * - Handling death (removing dead entities)
     * - Speciation (grouping entities by genome similarity)
     * - Fitness tracking and evaluation
     * - Generation counting (incremented on each birth)
     */
    class PopulationManager {
    public:
        PopulationManager() = default;
        ~PopulationManager() = default;
        neat::InnovationTracker innovation_tracker;  ///< Tracks innovation numbers for NEAT

        /**
         * @brief Initialize the population with random genomes.
         * @param population_size Number of initial entities to spawn
         * @param grid Reference to the hex grid for entity placement
         * @param num_inputs NEAT network input count (should be EntityInput::NUM_INPUTS)
         * @param num_outputs NEAT network output count (should be EntityOutput::NUM_OUTPUTS)
         */
        void initialize(int population_size, mapGen::HexGrid& grid,
                       int num_inputs, int num_outputs);

        /**
         * @brief Update all entities for one simulation frame.
         * @param delta_time Time step (in seconds)
         * @param grid Reference to the hex grid for movement/sensing
         * @param current_time Absolute simulation time (used for speciation timing)
         */
        void update(float delta_time, mapGen::HexGrid& grid, float current_time);

        /**
         * @brief Get population statistics for logging/visualization.
         * @return String containing population info (size, generation, species count, etc.)
         */
        std::string get_statistics() const;

        /**
         * @brief Get current generation count.
         * @return Number of births that have occurred (incremented each reproduction)
         */
        int get_generation_count() const { return generation_count; }

        /**
         * @brief Get number of species.
         * @return Count of current species groups
         */
        int get_species_count() const { return species.size(); }

        /**
         * @brief Get population size.
         * @return Count of living entities
         */
        size_t get_population_size() const { return entities.size(); }

        /**
         * @brief Get reference to all entities.
         */
        const std::vector<std::unique_ptr<Entity>>& get_entities() const { return entities; }

        /**
         * @brief Get reference to all species.
         */
        const std::vector<neat::Species>& get_species() const { return species; }

    private:
        std::vector<std::unique_ptr<Entity>> entities;
        std::vector<neat::Species> species;

        int generation_count = 0;           ///< Incremented each time an entity is born
        int num_inputs = 0;                 ///< NEAT network input count
        int num_outputs = 0;                ///< NEAT network output count

        float last_speciation_time = 0.0f;  ///< Track when last speciation occurred
        float speciation_interval = 5.0f;   ///< Speciate every N seconds

        /**
         * @brief Update a single entity's state for one frame.
         * Includes: sensory input gathering, brain activation, behavior execution,
         * fitness update, and state changes (eat, reproduce, die).
         */
        void update_entity(Entity& entity, mapGen::HexGrid& grid, float delta_time);

        /**
         * @brief Create a new offspring from parents.
         * Called when parents reproduce.
         * The offspring inherits the parents' genome via crossover.
         * @param parent First parent
         * @param parent2 Second parent
         * @param grid Grid to spawn offspring into
         * @param new_cohort_id Generation ID to assign to offspring
         * @return Newly created offspring entity
         */
        std::unique_ptr<Entity> create_offspring(Entity& parent, Entity& parent2, mapGen::HexGrid& grid, int new_cohort_id);

        /**
         * @brief Remove all dead entities from the population.
         * Called after updating all entities.
         */
        void remove_dead_entities(mapGen::HexGrid& grid);

        /**
         * @brief Handle reproduction for entities that want to reproduce.
         * Checks each entity's wants_to_reproduce flag, finds a mate,
         * deducts energy cost, and creates offspring.
         * Called after updating entities.
         */
        void handle_reproduction(mapGen::HexGrid& grid);

        /**
         * @brief Group entities into species based on genome distance.
         * Calculates genetic distance between each entity's genome.
         * Entities with distance < threshold are placed in same species.
         * Called periodically (every speciation_interval seconds).
         */
        void speciate_population();

        /**
         * @brief Kill off low-performing species.
         * Species with below-average fitness are culled (some/all removed).
         * Called after fitness evaluation and speciation.
         */
        void cull_weak_species();

        /**
         * @brief Calculate and update fitness scores for all entities.
         * Fitness typically based on: time alive, resources consumed, energy efficiency.
         * Called each frame.
         */
        void update_fitness_scores(float delta_time);

        /**
         * @brief Find closest entity for mating.
         * Looks for compatible mate (same or nearby species) within range.
         * @param entity Entity looking for a mate
         * @return Pointer to mate, or nullptr if none found nearby
         */
        Entity* find_potential_mate(const Entity& entity);

        /**
         * @brief Check if two genomes are in same species.
         * Uses genetic distance formula: sum of weight differences + penalty for topological differences.
         * @param genome1 First genome
         * @param genome2 Second genome
         * @return Distance metric (lower = more similar)
         */
        static float calculate_genetic_distance(const neat::Genome& genome1,
                                               const neat::Genome& genome2);
    };
}

