#include "Crossover.h"

namespace neat {
    Genome crossover(Genome &parent1, Genome &parent2) {
        // Assuming parent1.fitness > parent2.fitness
        Genome child;
        child.nodes = parent1.nodes;

        // Parent connection pointers.
        size_t i = 0;
        size_t j = 0;

        // 1. Traverse over connection lists.
        // 2. If innovation numbers match, randomly select one of the connections.
        // 3. If the link is disabled for a parent, keep it disabled for the child for 3 out of 4 times.
        // 4. If parent1 has a connection that parent2 doesn't, add it to the child.
        // 5. If parent2 has a connection that parent1 doesn't, don't add it to the list, as parent1 is fitter.
        while (i < parent1.connections.size() && j < parent2.connections.size()) {
            const auto& conn1 = parent1.connections[i];
            const auto& conn2 = parent2.connections[j];

            if (conn1.innovation_number == conn2.innovation_number) {
                // 2
                if (static_cast<double>(rand()) / RAND_MAX < 0.5) {
                    child.connections.push_back(conn1);
                } else {
                    child.connections.push_back(conn2);
                }

                // 3
                if (!conn1.enabled || !conn2.enabled) {
                    if (static_cast<double>(rand()) / RAND_MAX < 0.75) {
                        child.connections.back().enabled = false;
                    }
                }
                i++;
                j++;
            }

            // 4
            else if (conn1.innovation_number < conn2.innovation_number) {
                child.connections.push_back(conn1);
                i++;
            }

            // 5
            else {
                j++;
            }
        }

        // Add the remaining connections to the child.
        while (i < parent1.connections.size()) {
            child.connections.push_back(parent1.connections[i]);
            i++;
        }

        return child;
    }
}