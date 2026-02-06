#pragma once
#include <unordered_map>
#include "Hex.h"
#include "Cell.h"
#include "Common.h"

namespace mapGen {
    /**
       @brief Manages a collection of hex Cells organized in concentric rings around the origin.
       @details The grid stores Cells keyed by cube coordinates (Hex) and provides helpers
                for generating rings, iterating in a spiral order and simple retrieval.
                Important: radius == 0 represents the single center hex at (0,0,0).
    */
    class HexGrid {
    private:
        std::unordered_map<Hex, Cell, HexHash> map;

        /**
           @brief Generate and insert all cells that lie exactly 'radius' steps from center.
           @param center Center coordinate around which the ring is formed.
           @param radius Non-negative ring distance. If radius == 0, only the center cell is inserted.
           @throws std::invalid_argument when radius < 0.
           @details Inserts default-constructed Cell objects into the internal map for every
                    coordinate at exactly 'radius' steps away from the provided center.
        */
        void generateRing(const Hex &center, int radius);

    public:
        int _radius;

        /**
           @brief Build the world by generating rings from 0 up to the given radius.
           @param radius Non-negative maximum distance (in hex steps) to generate.
           @throws std::invalid_argument when radius < 0.
           @details After calling, the map contains every cell whose distance from the origin
                    is <= radius. radius == 0 will create only the origin cell.
        */
        void generateWorld(int radius);

        // Get
        /**
           @brief Return a pointer to the Cell at the specified hex coordinate.
           @param h The Hex coordinate to query.
           @returns Pointer to the Cell. If the coordinate does not exist, a default Cell is
                    inserted into the internal map and a pointer to that new element is returned.
        */
        Cell *getCell(const Hex &h);

        /**
           @brief Append all coordinates exactly 'radius' steps from center into out_results.
           @param center Center of the ring.
           @param radius Non-negative ring distance. If radius == 0, the center is appended.
           @param out_results Vector that will receive the ring coordinates (appended).
           @details The order of appended coordinates follows the internal ALL_DIRECTIONS ordering.
        */
        void getRing(const Hex &center, int radius, std::vector<Hex> &out_results) const;

        /**
           @brief Append coordinates in spiral order from radius 0 up to the specified radius.
           @param center Center of the spiral.
           @param radius Non-negative maximum ring distance to include.
           @param out_results Vector that will receive coordinates in spiral order (appended).
           @details Rings are appended sequentially starting with radius 0 (the center).
        */
        void getSpiral(const Hex &center, int radius, std::vector<Hex> &out_results) const;

        /**
           @brief Access the internal map of Hex->Cell.
           @returns Reference to the internal unordered_map storing grid cells.
           @details Allows external inspection or bulk operations; modifications affect the grid.
        */
        std::unordered_map<Hex, Cell, HexHash> &getMap();
    };
}
