// Separated from Genome.h for better modularity

#pragma once

namespace neat {

    enum class NodeType {
        INPUT,
        HIDDEN,
        OUTPUT,
        BIAS

    };

    struct NodeGene {
        int id = -1;
        NodeType type = NodeType::HIDDEN;

        // Constructors
        NodeGene() = default;
        NodeGene(int id_, NodeType type_) : id(id_), type(type_) {}

        // Comparison operators based on id. C++20 automatically provides !=.
        bool operator==(NodeGene const& o) const noexcept { return id == o.id; }
        bool operator<(NodeGene const& o) const noexcept { return id < o.id; }
    };

    struct ConnectionGene {
        int in_id = -1;
        int out_id = -1;
        int innovation_number = -1;
        double weight = 0.0;
        bool enabled = true;

        // Constructors
        ConnectionGene() = default;
        ConnectionGene(int in_id_, int out_id_, int innovation_number_, double weight_, bool enabled_)
            : in_id(in_id_), out_id(out_id_), innovation_number(innovation_number_), weight(weight_), enabled(enabled_) {}

        // Comparison operators based on innovation_number.
        bool operator==(ConnectionGene const& o) const noexcept { return innovation_number == o.innovation_number; }
        bool operator!=(ConnectionGene const& o) const noexcept { return innovation_number != o.innovation_number; }
        bool operator<(ConnectionGene const& o) const noexcept { return innovation_number < o.innovation_number; }
        bool operator>(ConnectionGene const& o) const noexcept { return innovation_number > o.innovation_number; }
    };
} // namespace neat