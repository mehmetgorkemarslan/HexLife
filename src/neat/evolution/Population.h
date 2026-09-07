#pragma once
#include <vector>

#include "../genotype/Genome.h"

namespace neat {
	class Population {
	public:
		Population() = default;
		~Population() = default;

		void initialize(int population_size, int num_inputs, int num_outputs);
		void update_population();
	private:
		int generation_count = 0; // This seems problematic, as we don't have any discrete generations.
		std::vector<Genome> genomes;
		// species list here after implementing it

		void speciate();
		void reproduce();
	};
} // namespace neat
