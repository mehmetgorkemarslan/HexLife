#include "Network.h"
#include <cmath>
#include <vector>

namespace neat {

	Network::Network(std::vector<Node> const &nodes_,
                 std::vector<ConnectionGene> const &genome_) noexcept
				: nodes(nodes_) {
		incoming.resize(nodes.size());
		outgoing.resize(nodes.size());
		// build id_to_idx mapping
		for (int i = 0; i < nodes.size(); i++) {
			id_to_idx[nodes[i].id] = i;
		}
		// translate connection genes to connections to avoid lookup later.
		for (const ConnectionGene gene : genome_) {
			if (!gene.enabled)
				continue;
			int in_idx = id_to_idx[gene.in_id];
			int out_idx = id_to_idx[gene.out_id];

			connections.push_back(Connection(in_idx, out_idx, gene.weight, true));
			incoming[out_idx].push_back(connections.size() - 1);
			outgoing[in_idx].push_back(connections.size() - 1);
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
					int out_idx = connections[conn_idx].out_idx;
					in_degree[out_idx]--;
					if (in_degree[out_idx] == 0) {
						next.push_back(out_idx);
					}
				}
			}
			curr = next;
		}

		// cycle check
		int processed_nodes = 0;
		for (const auto& layer : layers) {
			processed_nodes += layer.size();
		}

		// if a cycle is detected
		if (processed_nodes < nodes.size()) {
			std::vector<int> fallback_layer;
			std::vector<bool> is_processed(nodes.size(), false);
			for (const auto& layer : layers) {
				for (int idx : layer) {
					is_processed[idx] = true;
				}
			}
			for (int i=0; i<nodes.size(); i++) {
				if (is_processed[i]) {
					fallback_layer.push_back(i);
				}
			}
			if (!fallback_layer.empty()) {
				layers.push_back(fallback_layer);
			}
		}
	}

	// just a helper function to construct a Network from a Genome.
	Network Network::construct_from_genome(Genome const &genome) {
		std::vector<Node> nodes;
		for (const NodeGene &node : genome.nodes) {
			Node n(node.id);
			n.type = node.type;
			nodes.push_back(n);
		}

		Network network(nodes, genome.connections);
		network.build_layers();
		return network;
	}

	std::vector<double> Network::get_outputs() const {
		std::vector<std::pair<int, double>> output_nodes;
		for(const Node& node : nodes){
			if(node.type == NodeType::OUTPUT){
				output_nodes.push_back({node.id, node.output});
			}
		}
		std::sort(output_nodes.begin(), output_nodes.end(), 
				[](const std::pair<int, double>& a, const std::pair<int, double>& b) { return a.first < b.first; });

		std::vector<double> out;
		out.reserve(output_nodes.size());
		for(const auto& pair : output_nodes){
			out.push_back(pair.second);
		}

		return out;
	}

	void Network::set_inputs(std::unordered_map<int, double> const &input_values) {
		for (auto const &[node_id, value] : input_values) {
			Node &node = nodes[id_to_idx[node_id]];
			if (node.type == NodeType::INPUT) {
				node.output = value;
			}
		}
	}

	void Network::activate() {
		for (std::vector<int> &layer : layers) {
			for (int node_idx : layer) {
				Node &node = nodes[node_idx];

				if (node.type == NodeType::INPUT) continue;
				if (node.type == NodeType::BIAS) {
					node.output = 1;
					continue;
				}

				double curr_sum = 0;
				for (int conn_idx : incoming[node_idx]) {
					const Connection &conn = connections[conn_idx];
					if (conn.enabled) {
						curr_sum += nodes[conn.in_idx].output * conn.weight;
					}
				}
				node.output = std::tanh(curr_sum);
			}
		}
	}

	void Network::deactivate() {
		for (Node &node : nodes) {
			if (node.type != NodeType::INPUT) {
				node.output = 0;
			}
		}
	}
} // namespace neat