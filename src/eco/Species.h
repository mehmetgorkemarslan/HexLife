#pragma once

#include "Entity.h"

namespace eco {
    class HerbivoreOne : public Entity {
	public:
		HerbivoreOne() {
			maxEnergy = 100.0f;
			maxSatedness = 100.0f;
			maxHydratedness = 100.0f;

			energy = 100.0f;
			satedness = 100.0f;
			hydratedness = 100.0f;

			energy_drain_rate = 1.0f;
			reproduction_threshold = 180.0f;
			reproduction_cooldown = 10.0f;
			movement_speed = 5.0f;
			sensory_range = 5.0f;
			metabolism_cost = 1.0f;
			reproduction_cost = 50.0f;

			food_chain_rank = 1;
		}
	};

    
}