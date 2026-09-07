#include "Population.h"

namespace neat {
	void Population::initialize(int population_size, int num_inputs, int num_outputs) {
		genomes.resize(population_size);
		for (int i = 0; i < population_size; i++) {
			// Initialize genomes here.
		}
	}
	void Population::update_population() {
		// We need to define the behavior of agents before we can run asynch generations.
		return;
	}

	void Population::speciate() {

	}

	void Population::reproduce() {
		
	}
}