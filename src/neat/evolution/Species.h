#pragma once

#include <vector>
#include "../genotype/Genome.h"

namespace neat {
    struct Species {
        std::vector<Genome> genomes;
        Genome representative;
    };
}