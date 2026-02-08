#pragma once
#include <string>
#include "Common.h"

namespace mapGen {

    /**
     * @brief Represents a biome type with identity, display name, color and stats.
     */
    class Biome {
    public:
        /**
         * @brief Construct a Biome instance.
         * @param id Numeric identifier for this biome.
         * @param name Human-readable name for the biome. Must be same in config file
         * @param color RGB color used for visualization.
         * @param stats BiomeStats containing movement cost and fertility.
         */
        Biome(BiomeID id, std::string name, Color color, BiomeStats stats) : id(id), name(name),
            color(color), stats(stats) {
        }

        // Get
        /**
         * @brief Get the biome identifier.
         * @return The BiomeID of this biome.
         */
        BiomeID getBiomeID() const { return id; }

        /**
         * @brief Get the biome name.
         * @return Const reference to the biome name string.
         */
        const std::string &getName() const { return name; }

        /**
         * @brief Get the biome color.
         * @return Color struct representing the biome color.
         */
        Color getColor() const { return color; }

        /**
         * @brief Get the biome stats.
         * @return BiomeStats for movement cost and fertility.
         */
        BiomeStats getBiomeStats() const { return stats; }

    private:
        BiomeID id;
        std::string name;
        Color color;
        BiomeStats stats;
    };
}
