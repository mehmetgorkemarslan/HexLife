#pragma once

#include <algorithm>
#include <vector>
#include "Gene.h"

namespace neat {
    struct Genome {
        std::vector<NodeGene> nodes;
        std::vector<ConnectionGene> connections;
    };

    inline void sort_genes(Genome &genome) {
        std::sort(genome.nodes.begin(), genome.nodes.end());
        std::sort(genome.connections.begin(), genome.connections.end());
    }

    // Find connection using innovation_number.
    inline int find_connection(Genome &genome, int innovation) {
        for (size_t i = 0; i < genome.connections.size(); ++i) {
            if (genome.connections[i].innovation_number == innovation) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }
} // namespace neat