#include "PopulationManager.h"
#include "EntityIO.h"
#include "../neat/phenotype/Network.h"
#include "../neat/genotype/Genome.h"
#include "../neat/genotype/Gene.h"
#include "../neat/evolution/Crossover.h"
#include <iostream>
#include <algorithm>
#include <cmath>

#include "evolution/Mutator.h"
#include "evolution/Species.h"

namespace eco {

void PopulationManager::initialize(int population_size, mapGen::HexGrid& grid,
                                   int inputs, int outputs) {
    num_inputs = inputs;
    num_outputs = outputs;
    generation_count = 0;

    std::vector<mapGen::Hex> valid_positions;
    for (const auto& [hex, cell] : grid.getMap()) {
        valid_positions.push_back(hex);
    }

    for (int i = 0; i < population_size; ++i) {
        neat::Genome genome = neat::create_initial_genome(num_inputs, num_outputs, innovation_tracker);
        auto entity = std::make_unique<Entity>(genome, generation_count);
        
        mapGen::Hex pos{0, 0, 0};
        if (!valid_positions.empty()) {
            pos = valid_positions[rand() % valid_positions.size()];
        }
        entity->setPosition(pos);
        
        mapGen::Cell* cell = grid.getCell(pos);
        if (cell) {
            cell->addEntity(entity.get());
        }

        entities.push_back(std::move(entity));
    }

    std::cout << "[PopulationManager] Initialized with " << population_size
              << " entities (" << num_inputs << " inputs, " << num_outputs << " outputs)\n";
}

void PopulationManager::update(float delta_time, mapGen::HexGrid& grid, float current_time) {

     for (auto& entity : entities) {
         if (entity) {
             update_entity(*entity, grid, delta_time);
         }
     }

     update_fitness_scores(delta_time);

     // Handle reproduction for entities that want to reproduce
     handle_reproduction(grid);

     remove_dead_entities(grid);

     if (current_time - last_speciation_time > speciation_interval) {
         speciate_population();
         last_speciation_time = current_time;
     }
}

void PopulationManager::update_entity(Entity& entity, mapGen::HexGrid& grid, float delta_time) {
    entity.update(delta_time, grid);
}

void PopulationManager::remove_dead_entities(mapGen::HexGrid& grid) {
     for (const auto& entity : entities) {
         if (entity && !entity->getAlive()) {
             mapGen::Cell* cell = grid.getCell(entity->getPosition());
             if (cell) {
                 cell->removeEntity(entity.get());
             }
         }
     }

     entities.erase(std::remove_if(entities.begin(), entities.end(),
         [](const std::unique_ptr<Entity>& e) { return !e->getAlive(); }
     ), entities.end());
}

void PopulationManager::handle_reproduction(mapGen::HexGrid& grid) {
    std::vector<std::unique_ptr<Entity>> new_offspring;

    // Use indexed iteration over current population snapshot to avoid iterator invalidation
    size_t current_count = entities.size();
    for (size_t i = 0; i < current_count; ++i) {
        auto& entity = entities[i];
        if (entity && entity->getAlive() && entity->getWantsToReproduce()) {
            Entity* mate = find_potential_mate(*entity);
            if (mate) {
                // Deduct reproduction cost from both parents
                entity->setEnergy(entity->getEnergy() - entity->getReproductionCost());
                mate->setEnergy(mate->getEnergy() - mate->getReproductionCost());

                // Reset cooldowns
                entity->setReproductionCooldownTimer(entity->getReproductionCooldown());
                mate->setReproductionCooldownTimer(mate->getReproductionCooldown());

                // Clear reproduction desire for mate as well
                mate->setWantsToReproduce(false);

                // Create offspring
                auto offspring = create_offspring(*entity, *mate, grid, generation_count);
                if (offspring) {
                    new_offspring.push_back(std::move(offspring));
                }
            }

            // Clear the flag regardless (couldn't find mate or already mated)
            entity->setWantsToReproduce(false);
        }
    }

    // Append newly born offspring
    for (auto& child : new_offspring) {
        entities.push_back(std::move(child));
    }
}

std::unique_ptr<Entity> PopulationManager::create_offspring(Entity& parent, Entity& parent2, mapGen::HexGrid& grid, int new_cohort_id) {

    neat::Genome offspring_genome = neat::crossover(const_cast<neat::Genome&>(parent.getGenome()), const_cast<neat::Genome&>(parent2.getGenome()));
    auto offspring = std::make_unique<Entity>(offspring_genome, new_cohort_id);
    offspring->setPosition(parent.getPosition());
    
    // Register offspring with the grid cell
    mapGen::Cell* cell = grid.getCell(parent.getPosition());
    if (cell) {
        cell->addEntity(offspring.get());
    }

    generation_count++;
    return offspring;
}

void PopulationManager::speciate_population() {
    species.clear();

    for (auto& entity : entities) {
        if (!entity || !entity->getAlive()) continue;
        bool found_species = false;
        for (auto& sp : species) {
            float distance = calculate_genetic_distance(entity->getGenome(), sp.representative);
            if (distance < 3.0f) {
                sp.members.push_back(entity.get());
                found_species = true;
                break;
            }
        }
        if (!found_species) {
            neat::Species new_species;
            new_species.id = species.size();
            new_species.representative = entity->getGenome();
            new_species.members.push_back(entity.get());
            species.push_back(new_species);
        }
    }


    std::cout << "[PopulationManager] Speciation: " << species.size()
              << " species found\n";
}

void PopulationManager::cull_weak_species() {
    // I will add this here to get rid of weak species, as currently I don't have
    // any random death mechanism, so the population will just grow indefinitely.
    // The only problem is the absence of food or water, so to not clog the gene
    // pool with weak species, I will cull them based on fitness for now.

    std::vector<neat::Species> weak_species;
    for (const auto& sp : species) {
        float total_fitness = 0.0f;
        for (const auto& member : sp.members) {
            total_fitness += member->get_fitness();
        }
        // TODO: finish cull logic
    }
}

void PopulationManager::update_fitness_scores(float delta_time) {
    for (auto& entity : entities) {
        if (entity) {
            entity->update_fitness(delta_time);
        }
    }
}

Entity* PopulationManager::find_potential_mate(const Entity& entity) {
    if (entity.getAlive() && entity.getEnergy() > 40.0f) {
        for (const auto& potential_mate : entities) {
            if (potential_mate && potential_mate.get() != &entity && potential_mate->getAlive() &&
                potential_mate->getEnergy() > 40.0f) {
                // Check if they are in the same species or compatible
                float distance = calculate_genetic_distance(entity.getGenome(), potential_mate->getGenome());
                if (distance < 3.0f) { // Arbitrary compatibility threshold
                    return potential_mate.get();
                }
            }
        }
    }
    return nullptr;
}

float PopulationManager::calculate_genetic_distance(const neat::Genome& genome1,
                                                    const neat::Genome& genome2) {
    int disjoint = 0;
    float avg_weight_diff = 0.0f;

    for (const auto& gene1 : genome1.connections) {
        auto it = std::find_if(genome2.connections.begin(), genome2.connections.end(),
                               [&gene1](const neat::ConnectionGene& gene2) {
                                   return gene1.innovation_number == gene2.innovation_number;
                               });
        if (it == genome2.connections.end()) {
            disjoint++;
        } else {
            avg_weight_diff += std::abs(gene1.weight - it->weight);
        }
    }

    int distance = disjoint; // For simplicity, ignoring excess genes for now
    if (!genome1.connections.empty()) {
        avg_weight_diff /= genome1.connections.size();
    }

    return distance + avg_weight_diff;
}

std::string PopulationManager::get_statistics() const {
    return "[PopulationManager] Population: " + std::to_string(entities.size()) +
           " | Generation: " + std::to_string(generation_count) +
           " | Species: " + std::to_string(species.size());
}

}

