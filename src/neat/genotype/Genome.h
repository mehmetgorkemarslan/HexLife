#pragma once

#include <algorithm>
#include <cstdlib>
#include <vector>
#include "Gene.h"
#include "../evolution/InnovationTracker.h"

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
    // Can be swapped with binary search later on.
    inline int find_connection(Genome &genome, int innovation) {
        for (size_t i = 0; i < genome.connections.size(); ++i) {
            if (genome.connections[i].innovation_number == innovation) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

	inline Genome create_initial_genome(int num_inputs, int num_outputs, InnovationTracker& tracker){
		Genome genome;
		for(int i = 0; i < num_inputs; i++){
			NodeType type = (i == num_inputs - 1) ? NodeType::BIAS : NodeType::INPUT;
			genome.nodes.emplace_back(i, type);
		}

		for(int i = 0; i < num_outputs; i++){
			int out_id = num_inputs + i;
			genome.nodes.emplace_back(out_id, NodeType::OUTPUT);
		}

		for(int i = 0; i < num_inputs; i++){
			for(int j = 0; j < num_outputs; ++j){
				int out_id = num_inputs + j;
				int innov_id = tracker.get_connection_innovation(i, out_id);
				double weight = ((static_cast<double>(rand()) / RAND_MAX) * 2.0) - 1.0;
				genome.connections.emplace_back(i, out_id, innov_id, weight, true);
			}
		}

		sort_genes(genome);
		return genome;
	}

	// Crossover is declared in evolution/Crossover.h
} // namespace neat