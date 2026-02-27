#pragma once

#include <vector>

#include "../genotype/Genome.h"

namespace neat {
    class Mutator {
    public:
        Mutator() = default;
        ~Mutator() = default;

        void mutate_weights(Genome &genome);
        void add_node(Genome &genome);
        void add_connection(Genome &genome);
    };
}