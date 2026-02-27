#pragma once
#include <vector>

#include "../genotype/Gene.h"

namespace neat {

    struct Node {
        int id = -1;
        // int layer = -1;
        double input_sum = 0.0;
        double output = 0.0;
        NodeType type = NodeType::INPUT;

        // Constructors
        Node() = default;
        Node(int id_) : id(id_) {}

        // Destructor
        ~Node() = default;

        //std::vector<int> incoming_connections;
    };
} // namespace neat