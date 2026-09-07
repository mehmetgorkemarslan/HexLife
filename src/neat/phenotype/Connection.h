#pragma once

namespace neat {
	struct Connection {
		int in_idx = -1;
		int out_idx = -1;
		double weight = 0.0;
		bool enabled = true;

		// Constructors
		Connection() = default;
		Connection(int in_idx_, int out_idx_, double weight_, bool enabled_)
			: in_idx(in_idx_), out_idx(out_idx_), weight(weight_), enabled(enabled_) {}

		// Destructor
		~Connection() = default;
	};
} // namespace neat