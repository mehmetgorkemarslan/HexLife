#pragma once

#include <unordered_map>
#include <vector>
#include "Node.h"
#include "Connection.h"
#include  "../genotype/Genome.h"

namespace neat {
    class Network {
    public:
        // Constructors
        Network() = default;
        Network(std::vector<Node> const& nodes_, std::vector<Connection> const& connections_) noexcept;
        static Network construct_from_genome(Genome const& genome);

        // Destructor
        ~Network() = default;

        void set_inputs(std::unordered_map<int, double> const& input_values);
        void activate();
        void deactivate();

        std::vector<double> get_outputs() const;

    private:
        std::vector<Node> nodes;
        std::vector<Connection> connections;

        std::unordered_map<int, int> id_to_idx;

        std::vector<std::vector<int>> incoming;
        std::vector<std::vector<int>> outgoing;

        std::vector<std::vector<int>> layers;
        std::vector<std::vector<int>> conn_indices;

        void build_layers();

    };
} // namespace neat