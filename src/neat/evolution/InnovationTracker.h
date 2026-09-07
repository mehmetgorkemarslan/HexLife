#pragma once
#include <map>
#include <utility>

namespace neat {
    class InnovationTracker {
	public:
	// Returns the innovation number for a connection between in_id and out_id
		int get_connection_innovation(int in_id, int out_id) {
            auto key = std::make_pair(in_id, out_id);
            auto it = connection_innovations.find(key);
            if (it != connection_innovations.end()) {
                return it->second;
            }
            int new_id = next_innovation_number++;
            connection_innovations[key] = new_id;
            return new_id;
        }
	// Returns the innovation number for a new node
		int get_node_innovation(int split_conn_innov) {
            auto it = node_innovations.find(split_conn_innov);
            if (it != node_innovations.end()) {
                return it->second;
            }
            int new_id = next_node_id++;
            node_innovations[split_conn_innov] = new_id;
            return new_id;
        }
	// Resets the innovation tracker to its initial state
		void reset(int initial_node_count, int initial_innov_count){
			next_node_id = initial_node_count ;
			next_innovation_number = initial_innov_count ;
			connection_innovations.clear();
			node_innovations.clear();
		}
	private:
		std::map<std::pair<int, int>, int> connection_innovations;
		std::map<int, int> node_innovations;
		int next_innovation_number = 0;
		int next_node_id = 0;
	};
}