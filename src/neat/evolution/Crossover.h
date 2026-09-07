// Declaration header for crossover implementation
#pragma once

#include "../genotype/Genome.h"

namespace neat {
    // Create a child genome by performing NEAT crossover between two parents.
    // parent1 is assumed to be the more-fit parent when using disjoint/excess gene rules.
    Genome crossover(Genome &parent1, Genome &parent2);
}

