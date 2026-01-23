#pragma once

namespace neat {
    struct Connection {
        int in_id = -1;
        int out_id = -1;
        double weight = 0.0;
        bool enabled = true;

        // Constructors
        Connection() = default;
        Connection(int in_id_, int out_id_, double weight_, bool enabled_)
            : in_id(in_id_), out_id(out_id_), weight(weight_), enabled(enabled_) {}

        // Destructor
        ~Connection() = default;
    };
} // namespace neat