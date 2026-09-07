#pragma once

#include "../genotype/Genome.h"
#include <vector>

namespace eco {
	class Entity;
}

namespace neat {
	struct Species {
		int id;
		std::vector<eco::Entity*> members;
		Genome representative;
	};
} // namespace neat