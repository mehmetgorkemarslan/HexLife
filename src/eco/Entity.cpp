#include "Entity.h"
#include "EntityIO.h"
#include "../neat/phenotype/Network.h"
#include <algorithm>
#include <cmath>
#include <vector>

namespace {
    float clamp01(float value) {
        return std::max(0.0f, std::min(1.0f, value));
    }

    template <typename Predicate>
    bool any_cell_in_spiral(mapGen::HexGrid& grid, const mapGen::Hex& center, int radius, Predicate&& predicate) {
        std::vector<mapGen::Hex> positions;
        grid.getSpiral(center, radius, positions);

        auto& cells = grid.getMap();
        for (const auto& hex : positions) {
            auto it = cells.find(hex);
            if (it != cells.end() && predicate(it->second)) {
                return true;
            }
        }

        return false;
    }
}

namespace eco {

    static int next_entity_id = 0;

    Entity::Entity() : id(next_entity_id++) {
		brain_genome.nodes.push_back(neat::NodeGene(0, neat::NodeType::INPUT));
		brain_genome.nodes.push_back(neat::NodeGene(1, neat::NodeType::OUTPUT));
	}


    Entity::Entity(const neat::Genome& genome, int cohort_id_in) 
	: id(next_entity_id++), cohort_id(cohort_id_in), brain_genome(genome){
		brain = std::make_unique<neat::Network>(neat::Network::construct_from_genome(genome));
		is_alive = true;
		time_alive = 0;
	}

    Entity::~Entity() = default;

    void Entity::update(float delta_time, mapGen::HexGrid& grid) {
		if(!is_alive) return;
		time_alive += delta_time;
		if (movement_cooldown_timer > 0.0f) {
			movement_cooldown_timer -= delta_time;
		}
		if (reproduction_cooldown_timer > 0.0f) {
			reproduction_cooldown_timer -= delta_time;
		}

		mapGen::Cell* current_cell = grid.getCell(hex_pos);

		if(brain){
			std::unordered_map<int, double> sensory_input = 
				gather_sensory_input(grid, current_cell);
			brain->set_inputs(sensory_input);
			brain->activate();

			std::vector<double> outputs = brain->get_outputs();
			interpret_outputs(outputs);
		}

		if(wants_to_move && movement_cooldown_timer <= 0.0f){
			move(grid, desired_move_direction);
			energy -= movement_speed_mult * metabolism_cost;
			movement_cooldown_timer = static_cast<float>(0.3f / std::max(0.1, movement_speed_mult));
			wants_to_move = false;
		}

		energy -= energy_drain_rate * delta_time;
		if(energy <= 0.0f){
			die();
		}
	}

    void Entity::move(mapGen::HexGrid &grid, mapGen::HexDirection direction) {
        // Determine which hex cell is one step in the requested direction.
        mapGen::Hex target = mapGen::Hex::get_neighbor(hex_pos, direction);

        // Bounds check against grid radius
        if (mapGen::Hex::length(target) > grid._radius) {
            return;
        }

        // Retrieve the target cell
        mapGen::Cell *target_cell = grid.getCell(target);
        if (!target_cell) return;

        // Remove self from the current cell.
        mapGen::Cell *current_cell = grid.getCell(hex_pos);
        if (current_cell) current_cell->removeEntity(this);

        // Step into the new position.
        hex_pos = target;
        target_cell->addEntity(this);
    }

	std::unordered_map<int, double> Entity::gather_sensory_input(mapGen::HexGrid& grid, const mapGen::Cell* current_cell) const {
		std::unordered_map<int, double> sensory_input;
		// Energy/Resources as normalized percentages
    	sensory_input[input_to_int(EntityInput::ENERGY_LEVEL)] = 
        clamp01(energy / maxEnergy);

    	sensory_input[input_to_int(EntityInput::SATEDNESS)] = 
        clamp01(satedness / maxSatedness);

    	sensory_input[input_to_int(EntityInput::HYDRATEDNESS)] = 
        clamp01(hydratedness / maxHydratedness);

	    sensory_input[input_to_int(EntityInput::FOOD_NEARBY)] = detect_nearby_food(grid, current_cell) ? 1.0 : 0.0;
	    sensory_input[input_to_int(EntityInput::WATER_NEARBY)] = detect_nearby_water(grid, current_cell) ? 1.0 : 0.0;
	    sensory_input[input_to_int(EntityInput::PREDATOR_NEARBY)] = detect_nearby_predator(grid, current_cell) ? 1.0 : 0.0;
	    sensory_input[input_to_int(EntityInput::MATE_NEARBY)] = detect_nearby_mate(grid, current_cell) ? 1.0 : 0.0;

    // Bias input (always 1.0)
    	sensory_input[input_to_int(EntityInput::BIAS)] = 1.0;
    
    return sensory_input;
	}

	void Entity::interpret_outputs(const std::vector<double>& outputs) {
		if (outputs.size() < static_cast<size_t>(EntityOutput::NUM_OUTPUTS)) {
			return;
		}

		int direction_idx = select_direction_from_output(outputs[output_to_int(EntityOutput::MOVE_DIRECTION)]);
		desired_move_direction = static_cast<mapGen::HexDirection>(direction_idx);
		movement_speed_mult = normalize_tanh(outputs[output_to_int(EntityOutput::MOVE_SPEED)]);
		wants_to_move = (movement_speed_mult > 0.2);

		double reproductiveness = normalize_tanh(outputs[output_to_int(EntityOutput::REPRODUCE_DESIRE)]);
		if(reproductiveness > 0.7 && energy > reproduction_threshold && reproduction_cooldown_timer <= 0.0f){
			wants_to_reproduce = true;
		}
	}

    void Entity::die() {
		is_alive = false;
	}

	// private methods
  bool Entity::detect_nearby_food(mapGen::HexGrid& grid, const mapGen::Cell* current_cell) const {
    const int radius = std::max(0, static_cast<int>(std::ceil(std::max(vision_range, sensory_range))));
    const auto has_food_biome = [](const mapGen::Cell& cell) {
      // Treat moderately wet, non-extreme cells as edible vegetation zones.
      return cell.moisture >= 0.35f && cell.moisture <= 0.80f && cell.temperature > -0.35f;
    };

    if (current_cell && has_food_biome(*current_cell)) {
      return true;
    }

    return any_cell_in_spiral(grid, hex_pos, radius, has_food_biome);
	}

  bool Entity::detect_nearby_water(mapGen::HexGrid& grid, const mapGen::Cell* current_cell) const {
    const int radius = std::max(0, static_cast<int>(std::ceil(std::max(vision_range, sensory_range))));
    const auto has_water = [](const mapGen::Cell& cell) {
      return cell.moisture >= 0.80f || cell.temperature <= -0.50f;
    };

    if (current_cell && has_water(*current_cell)) {
      return true;
    }

    return any_cell_in_spiral(grid, hex_pos, radius, has_water);
	}

  bool Entity::detect_nearby_predator(mapGen::HexGrid& grid, const mapGen::Cell* current_cell) const {
    const int radius = std::max(0, static_cast<int>(std::ceil(std::max(vision_range, sensory_range))));
    const auto has_predator = [this](const mapGen::Cell& cell) {
      for (const auto* other : cell.entities) {
        if (other && other != this && other->is_alive && other->food_chain_rank > food_chain_rank) {
          return true;
        }
      }
      return false;
    };

    if (current_cell && has_predator(*current_cell)) {
      return true;
    }

    return any_cell_in_spiral(grid, hex_pos, radius, has_predator);
	}
	
  bool Entity::detect_nearby_mate(mapGen::HexGrid& grid, const mapGen::Cell* current_cell) const {
    const int radius = std::max(0, static_cast<int>(std::ceil(std::max(vision_range, sensory_range))));
    const auto has_mate = [this](const mapGen::Cell& cell) {
      for (const auto* other : cell.entities) {
        if (other && other != this && other->is_alive && other->food_chain_rank == food_chain_rank) {
          return true;
        }
      }
      return false;
    };

    if (current_cell && has_mate(*current_cell)) {
      return true;
    }

    return any_cell_in_spiral(grid, hex_pos, radius, has_mate);
	}
}