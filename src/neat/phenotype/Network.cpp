#include "Network.h"
#include <cmath>

namespace neat {
    Network::Network(std::vector<Node> const &nodes_, std::vector<Connection> const &connections_) noexcept {

    }

    Network Network::construct_from_genome(Genome const &genome) {
        return {};
    }

    std::vector<double> Network::get_outputs() {
        // Placeholder
        return {};
    }

    void Network::set_inputs(std::unordered_map<int, double> const& input_values) {
        for (auto const& [node_id, value] : input_values) {
            Node& node = nodes[id_to_idx[node_id]];
            if (node.type == INPUT) {
                node.output = value;
            }
        }
    }

    void Network::activate() {
        for (std::vector<int>& layer : layers) {
            for (int node_id : layer) {
                Node& node = nodes[id_to_idx[node_id]];

                if (node.type == INPUT) {
                    continue;
                }

                for (int conn_idx : conn_indices[id_to_idx[node_id]]) {
                    Connection& conn = connections[conn_idx];
                    if (conn.enabled) {
                        Node& in_node = nodes[id_to_idx[conn.in_id]];
                        node.input_sum += in_node.output * conn.weight;
                    }
                }
            }
        }
    }

    void Network::deactivate() {
        for (Node& node: nodes) {
            if (node.type != INPUT) {
                node.input_sum = 0; node.output = 0;
            }
        }
    }
} // namespace neat