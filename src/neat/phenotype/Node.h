#pragma once
#include <vector>

namespace neat {

    enum NodeType {
        INPUT,
        HIDDEN,
        OUTPUT
    };

    struct Node {
        int id = -1;
        // int layer = -1;
        double input_sum = 0.0;
        double output = 0.0;
        double bias = 0.0;
        NodeType type = INPUT;

        // Constructors
        Node() = default;
        Node(int id_, double bias_) : id(id_), bias(bias_) {}

        // Destructor
        ~Node() = default;

        //std::vector<int> incoming_connections;
    };
} // namespace neat