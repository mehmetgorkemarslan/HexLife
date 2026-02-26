#include "Network.h"
#include <cmath>

namespace neat {
    Network::Network(std::vector<Node> const &nodes_, std::vector<Connection> const &connections_) noexcept
    : nodes(nodes_), connections(connections_) {
        incoming.resize(nodes.size());
        outgoing.resize(nodes.size());
        // build id_to_idx mapping
        for (int i = 0; i < nodes.size(); i++) {
            id_to_idx[nodes[i].id] = i;
        }
        // build incoming and outgoing connection indices to avoid repeated lookups during activation
        for (int i = 0; i < connections.size(); i++) {
            if (!connections[i].enabled) continue;
            const int in_idx = id_to_idx[connections[i].in_id];
            const int out_idx = id_to_idx[connections[i].out_id];
            incoming[out_idx].push_back(i);
            outgoing[in_idx].push_back(i);
        }
    }

    void Network::build_layers() {
        layers.clear();

        std::vector<int> in_degree(nodes.size(), 0);
        // calculate in-degrees for topological sorting
        for (int i = 0; i < nodes.size(); i++) {
            for (int conn_idx : incoming[i]) {
                if (connections[conn_idx].enabled) {
                    in_degree[i]++;
                }
            }
        }
        std::vector<int> curr;
        // start with nodes that have no incoming connections (input and bias nodes)
        for (int i = 0; i < nodes.size(); i++) {
            if (in_degree[i] == 0) {
                curr.push_back(i);
            }
        }
        // perform topological sort to determine layers (kahn)
        while (!curr.empty()) {
            layers.push_back(curr);
            std::vector<int> next;

            for (int node_idx : curr) {
                for (int conn_idx : outgoing[node_idx]) {
                    int out_idx = id_to_idx[connections[conn_idx].out_id];
                    in_degree[out_idx]--;
                    if (in_degree[out_idx] == 0) {
                        next.push_back(out_idx);
                    }
                }
            }
            curr = next;
        }
    }

    // just a helper function to construct a Network from a Genome.
    Network Network::construct_from_genome(Genome const &genome) {
        std::vector<Node> nodes;
        std::vector<Connection> connections;
        std::unordered_map<int, int> id_to_idx;
        for (const NodeGene& node : genome.nodes) {
            id_to_idx[node.id] = nodes.size();
            nodes.emplace_back(node.id, 0.0); // Bias is set to 0.0
            nodes.back().type = node.type;
        }
        for (const ConnectionGene& connection : genome.connections) {
            if (connection.enabled) {
                connections.emplace_back(connection.in_id, connection.out_id, connection.weight, connection.enabled);
            }
        }

        Network network(nodes, connections);
        network.build_layers();
        return network;
    }

    std::vector<double> Network::get_outputs() const {
        std::vector<double> outputs;
        for (const Node &node : nodes) {
            if (node.type == NodeType::OUTPUT) outputs.push_back(node.output);
        }
        return outputs;
    }

    void Network::set_inputs(std::unordered_map<int, double> const& input_values) {
        for (auto const& [node_id, value] : input_values) {
            Node& node = nodes[id_to_idx[node_id]];
            if (node.type == NodeType::INPUT) {
                node.output = value;
            }
        }
    }

    void Network::activate() {
        for (std::vector<int>& layer : layers) {
            for (int node_idx : layer) {
                Node& node = nodes[node_idx];

                if (node.type == NodeType::INPUT) continue;
                if (node.type == NodeType::BIAS) {
                    node.output = 1;
                    continue;
                }

                double curr_sum = 0.0;
                for (int conn_idx : incoming[node_idx]) {
                    Connection& conn = connections[conn_idx];
                    if (conn.enabled) {
                        // will remove lookup later.
                        int in_idx = id_to_idx[conn.in_id];
                        curr_sum += nodes[in_idx].output * conn.weight;
                    }
                }
                node.output = std::tanh(curr_sum + node.bias);
                node.input_sum = curr_sum;
            }
        }
    }

    void Network::deactivate() {
        for (Node& node: nodes) {
            if (node.type != NodeType::INPUT) {
                node.input_sum = 0; node.output = 0;
            }
        }
    }
} // namespace neat