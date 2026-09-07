#pragma once

#include "MapSystem/Hex.h"
#include "MapSystem/HexGrid.h"
#include "Common.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include "genotype/Genome.h"

namespace neat { class Network; }

namespace eco {


    class Entity {
    public:
        Entity();
		Entity(const neat::Genome& genome, int cohort_id_in);
        ~Entity();

        void update(float delta_time, mapGen::HexGrid& grid);
        void move(mapGen::HexGrid& grid, mapGen::HexDirection direction);
        void die();

		const mapGen::Hex& getPosition() const { return hex_pos; }
		const neat::Genome& getGenome() const { return brain_genome; }
		void setPosition(const mapGen::Hex& position) { hex_pos = position; }
    	const bool getAlive() const { return is_alive; }

		std::unordered_map<int, double> gather_sensory_input(mapGen::HexGrid& grid, const mapGen::Cell* current_cell) const;
		void interpret_outputs(const std::vector<double>& outputs);
		float get_fitness() const { return fitness; }
		void update_fitness(float delta_time) { fitness += delta_time; }

    	std::unique_ptr<neat::Network>& getBrain() { return brain; }

    protected:
        int id;
        mapGen::Hex hex_pos{0, 0, 0};  ///< Current position in cube coordinates.
        
        // Brain
		neat::Genome brain_genome;
        std::unique_ptr<neat::Network> brain;

        // Stats & Traits
        float energy = 100.0f, satedness = 100.0f, hydratedness = 100.0f;
        float maxEnergy = 100.0f, maxSatedness = 100.0f, maxHydratedness = 100.0f;
        float energy_drain_rate = 0.5f;
        float reproduction_threshold = 80.0f;
        float reproduction_cooldown = 10.0f;
        float movement_speed = 1.0f;
        float vision_range = 3.0f;
        float sensory_range = 3.0f;
        float metabolism_cost = 0.01f;
        float reproduction_cost = 40.0f;
        
        // Timers
        float reproduction_cooldown_timer = 0.0f;
        float movement_cooldown_timer = 0.0f;
        
        int food_chain_rank = 1;

		// NEAT related
		int cohort_id = 0;
		float fitness = 0.0f;

		// Behaviours
		bool wants_to_move = false;
		mapGen::HexDirection desired_move_direction;
		double movement_speed_mult = 1.0f;
		bool wants_to_reproduce = false;

		// Condition
		bool is_alive = true;
		float time_alive = 0.0f;

    public:
        int getId() const { return id; }
        int getCohort() const { return cohort_id; }
        float getEnergy() const { return energy; }
        float getMaxEnergy() const { return maxEnergy; }
        float getSatedness() const { return satedness; }
        float getHydratedness() const { return hydratedness; }
        float getTimeAlive() const { return time_alive; }
        bool getWantsToMove() const { return wants_to_move; }
        mapGen::HexDirection getDesiredMoveDirection() const { return desired_move_direction; }
        bool getWantsToReproduce() const { return wants_to_reproduce; }
        float getReproductionCost() const { return reproduction_cost; }
        float getReproductionCooldown() const { return reproduction_cooldown; }
    	void setWantsToReproduce(bool value) { wants_to_reproduce = value; }

        // Setters for PopulationManager
        void setEnergy(float value) { energy = value; }
        void setSatedness(float value) { satedness = value; }
        void setHydratedness(float value) { hydratedness = value; }
        void setReproductionCooldownTimer(float value) { reproduction_cooldown_timer = value; }

	private:
    bool detect_nearby_food(mapGen::HexGrid& grid, const mapGen::Cell* current_cell) const;
    bool detect_nearby_water(mapGen::HexGrid& grid, const mapGen::Cell* current_cell) const;
    bool detect_nearby_predator(mapGen::HexGrid& grid, const mapGen::Cell* current_cell) const;
    bool detect_nearby_mate(mapGen::HexGrid& grid, const mapGen::Cell* current_cell) const;
    };
}