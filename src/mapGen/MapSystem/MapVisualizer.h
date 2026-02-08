#pragma once
#include "HexGrid.h"
#include "Common.h"

namespace mapGen {
    class MapVisualizer {
    public:
        static void DisplayAllGrid(HexGrid &grid); // Legacy
        static void DisplayDisc(HexGrid &grid, const Hex &center, int radius); // Legacy

        static void exportToPNG(HexGrid &grid, const std::string &filename);

    private:
        static void addSpaces(std::stringstream &ss, int row, int radius); // Legacy helper

        static void drawHexagon(std::vector<uint8_t> &image, int imgWidth, int imgHeight, float cx, float cy, float size, Color color);
    };
};
