#pragma once
#include <unordered_map>
#include <sstream>
#include "Hex.h"
#include "Cell.h"

namespace mapGen {
    /**
     * @brief Manages cells stored in concentric rings and provides query and visualization helpers.
     *
     * Stores Cell objects keyed by Hex coordinates (cube). Provides utilities for creating
     * concentric rings, scanning areas in spiral order, and generating simple text-based visualizations.
     * All calculations assume the origin (0,0,0) as the default center.
     */
    class HexGrid {
    private:
        std::unordered_map<Hex, Cell, HexHash> map;

        /**
         * @brief Generates a single ring of cells at the given radius around center.
         *
         * Inserts default-constructed Cell objects into the internal map for every position
         * at exactly 'radius' steps from the provided center.
         *
         * @param center The center coordinate of the ring.
         * @param radius The distance in steps from center; must be > 0.
         * @throws std::invalid_argument if radius <= 0.
         */
        void generateRing(const Hex &center, int radius);

        /**
         * @brief Helper used by displayDisc to add row-leading spaces and a newline.
         *
         * Appends the appropriate number of space groups and a newline into the provided stringstream.
         * This function performs no validation and is purely formatting-focused.
         *
         * @param ss Destination stringstream for the formatted output.
         * @param row Current row index within the displayed disk (0..radius-1).
         * @param radius Radius of the disk being displayed.
         */
        void addSpaces(std::stringstream &ss, int row, int radius);

    public:
        int _radius;

        /**
         * @brief Builds the world by generating rings from 1 up to the given radius.
         *
         * After calling this, the grid will contain all cells whose distance from the origin
         * is less than or equal to the given radius.
         *
         * @param radius Maximum distance (in steps) to generate; must be > 0.
         * @throws std::invalid_argument if radius <= 0.
         */
        void generateWorld(int radius);

        // Get
        /**
         * @brief Returns a pointer to the Cell at the specified hex coordinate.
         *
         * If the coordinate does not exist in the map, operator[] will insert a default Cell,
         * so the returned pointer is always valid but may point to a newly created element.
         *
         * @param h The Hex coordinate to query.
         * @return Pointer to the corresponding Cell.
         */
        Cell *getCell(const Hex &h) ;

        /**
         * @brief Collects all hex coordinates that are exactly 'radius' steps from center.
         *
         * Coordinates are appended to out_results in a deterministic order that depends on the
         * library's direction ordering. radius must be > 0; for radius == 1 the center is appended.
         *
         * @param center Center of the ring.
         * @param radius Exact distance of the ring to collect; must be > 0.
         * @param out_results Vector to which the coordinates will be appended.
         */
        void getRing(const Hex &center, int radius, std::vector<Hex> &out_results) const;

        /**
         * @brief Appends coordinates in spiral order from 1 up to the specified radius.
         *
         * Rings are appended one by one; if radius is 0, nothing is added.
         *
         * @param center Center of the spiral.
         * @param radius Maximum ring distance to include.
         * @param out_results Vector to which the coordinates will be appended.
         */
        void getSpiral(const Hex &center, int radius, std::vector<Hex> &out_results) const;


        // Display
        // TODO: Use Buffered Logging for less print and log calls.
        /**
         * @brief Logs the entire generated grid using each Cell's getRepresentation method.
         *
         * A convenience helper that internally calls displayDisc for the stored world radius.
         */
        void displayGrid() ;

        /**
         * @brief Produces a multi-line ASCII-like representation of the area around center and logs it.
         *
         * Each cell's getRepresentation is concatenated into the final string which is then sent to the Logger.
         * If the requested disk exceeds the generated world bounds (_radius), an exception is thrown.
         *
         * @param center Center of the displayed area.
         * @param radius Radius of the disk to display (>=1 and within generated world bounds).
         * @throws std::invalid_argument if the requested disk is out of bounds.
         */
        void displayDisc(const Hex &center, int radius) ;
    };
}
