#include "Mutator.h"

namespace neat {

    void Mutator::mutate_weights(Genome &genome) {
        // Perturb 80 assign new weight 10 nothing 10
        for (ConnectionGene& conn : genome.connections) {
            double rand_val = static_cast<double>(rand()) / RAND_MAX;
            if (rand_val < 0.8) {
                conn.weight += (static_cast<double>(rand()) / RAND_MAX - 0.5) * 0.1;
            }else if (rand_val < 0.9) {
                conn.weight = static_cast<double>(rand()) / RAND_MAX * 2 - 1;
            }
        }
    }

    void Mutator::add_node(Genome &genome) {
        if (genome.connections.empty()) return;

        double rand_val = static_cast<double>(rand()) / RAND_MAX;
        if (rand_val < 0.9) return;

        int rand_conn_idx = rand() % genome.connections.size();
        ConnectionGene& conn = genome.connections[rand_conn_idx];

        if (!conn.enabled) return;
        conn.enabled = false;

        int new_node_id = genome.nodes.size();
        genome.nodes.emplace_back(new_node_id, NodeType::HIDDEN);

        genome.connections.emplace_back(conn.in_id, new_node_id, -1, 1.0, true);
        genome.connections.emplace_back(new_node_id, conn.out_id, -1, conn.weight, true);
    }

    void Mutator::add_connection(Genome &genome) {
    }
} // namespace neat
