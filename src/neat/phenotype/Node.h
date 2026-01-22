#pragma once
#include <vector>

namespace neat {
    struct Node {
        int id = -1;
        double value = 0.0;
        double bias = 0.0;

        // Constructors
        Node() = default;
        Node(int id_, double bias_) : id(id_), bias(bias_) {}

        std::vector<int> incoming_connections;
    };
} // namespace neat