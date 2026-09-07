#pragma once
#include <vector>

#include "../genotype/Gene.h"

namespace neat {

    struct Node {
        int id = -1;
        double output = 0.0;
        NodeType type = NodeType::INPUT;

        // Constructors
        Node() = default;
        Node(int id_) : id(id_) {}

        // Destructor
        ~Node() = default;
    };
} // namespace neat