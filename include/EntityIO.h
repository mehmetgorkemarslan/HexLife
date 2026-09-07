#pragma once

#include <algorithm>
namespace eco {
    /**
     * @brief Input layer definitions for Entity neural networks.
     * These correspond to NEAT network input nodes.
     * Total: 8 inputs
     */
    enum class EntityInput : int {
        ENERGY_LEVEL = 0,          ///< Current energy as % of max (0-1)
        SATEDNESS = 1,             ///< Current hunger satisfaction (0-1)
        HYDRATEDNESS = 2,          ///< Current hydration level (0-1)
        FOOD_NEARBY = 3,           ///< 1 if food detected in vision range, 0 otherwise
        PREDATOR_NEARBY = 4,       ///< 1 if predator detected, 0 otherwise
        WATER_NEARBY = 5,          ///< 1 if water detected, 0 otherwise
        MATE_NEARBY = 6,           ///< 1 if potential mate detected, 0 otherwise
        BIAS = 7,                  ///< Always 1.0 (bias input)

        NUM_INPUTS = 8             ///< Total number of input nodes
    };

    /**
     * @brief Output layer definitions for Entity neural networks.
     * These correspond to NEAT network output nodes.
     * Total: 3 outputs
     */
    enum class EntityOutput : int {
        MOVE_DIRECTION = 0,         ///< Which direction to move (0-5 → HexDirection)
                                    ///< Use argmax/softmax to select best direction
        MOVE_SPEED = 1,             ///< Movement speed multiplier (0-1)
                                    ///< Scales movement cost and energy drain
        REPRODUCE_DESIRE = 2,       ///< Reproduction urgency (0-1)
                                    ///< If > threshold and sufficient energy, reproduce

        NUM_OUTPUTS = 3             ///< Total number of output nodes
    };

    /**
     * @brief Map the output ids so that they don't collide with input ids.
     */
    inline int output_to_node_id(EntityOutput output) {
        return static_cast<int>(EntityInput::NUM_INPUTS) + static_cast<int>(output);
    }

	inline double normalize_tanh(double val){
		return (val + 1.0) * 0.5;
	}

	inline int select_direction_from_output(double direction_output){
		double norm = normalize_tanh(direction_output);
		int direction = static_cast<int>(norm * 6.0);
		return std::clamp(direction, 0, 5);
	}

    /**
     * @brief Helper function to convert EntityInput enum to int
     */
    inline int input_to_int(EntityInput input) {
        return static_cast<int>(input);
    }

    /**
     * @brief Helper function to convert EntityOutput enum to int
     */
    inline int output_to_int(EntityOutput output) {
        return static_cast<int>(output);
    }

}

