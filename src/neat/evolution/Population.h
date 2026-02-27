#pragma once
#include <vector>

#include "genotype/Genome.h"

namespace neat {
    class Population {
    public:
        Population() = default;
        ~Population() = default;

        void initialize(int population_size, int num_inputs, int num_outputs);
        void run_generation(); // main loop for running one generation of evolution, including speciation and reproduction.

    private:
        int generation_count = 0;
        std::vector<Genome> genomes;
        //species list here after implementing it

        void speciate();
        void reproduce();
    };
}
