#pragma once
#include <memory>
#include "HexGrid.h"
#include "NoiseGenerator.h"

namespace mapGen {
    /**
     * @brief Manages world-level generation tasks (temperature, moisture, biomes).
     * @details Uses configured Perlin noise generators and configuration offsets/seeds
     *          to populate cells in the provided HexGrid with temperature, moisture
     *          and biome information.
     */
    class WorldManager {
    public:
        /**
         * @brief Construct a WorldManager for the given HexGrid.
         * @param grid Pointer to the HexGrid that will be populated. Must not be null.
         */
        WorldManager(HexGrid *grid);

        /**
         * @brief Populate grid cells with temperature, moisture and biome.
         * @details Iterates all cells in the target grid, samples noise generators
         *          using configured scales and offsets, and assigns the computed
         *          temperature, moisture and biome to each cell.
         */
        void addBiomes();

    private:
        HexGrid *targetGrid;

        unsigned int tSeed;
        unsigned int mSeed;

        float tOffset;
        float mOffset;

        std::unique_ptr<NoiseGenerator> tempNoise;
        std::unique_ptr<NoiseGenerator> moistureNoise;
    };
}
