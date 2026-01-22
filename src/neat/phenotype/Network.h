#pragma once

#include <vector>
#include "Node.h"
#include "Connection.h"

namespace neat {
    class Network {
    public:
        // Constructors
        Network() = default;
        Network(std::vector<Node> const& nodes_, std::vector<Connection> const& connections_) noexcept;

        void set_inputs(std::vector<double> const& input_values);
        void activate();
        void deactivate();

        std::vector<double> get_outputs() const;

    private:
        std::vector<Node> nodes;
        std::vector<Connection> connections;
    };
} // namespace neat