#include "HexGrid.h"
#include "Logger.h"

namespace mapGen {
    void HexGrid::generateRing(const Hex &center, int radius) {
        if (radius < 0) {
            Logger::Log(LogLevel::ERROR, "[HexGrid] Ring radius must be non negative");
            throw std::invalid_argument("Ring radius must be non negative");
        }

        if (radius == 0) {
            map[center] = Cell();
            return;
        }

        Hex tempHex = center + (Hex{0, -1, 1} * (radius));


        for (auto dir: ALL_DIRECTIONS) {
            for (int step = 0; step < radius; step++) {
                map[tempHex] = Cell();
                tempHex = Hex::get_neighbor(tempHex, dir);
            }
        }
    }


    void HexGrid::generateWorld(int radius) {
        if (radius < 0) {
            Logger::Log(LogLevel::ERROR, "[HexGrid] World radius must be non negative");
            throw std::invalid_argument("World radius must be non negative");
        }
        this->_radius = radius;
        Hex center = {0, 0, 0};

        for (int i = 0; i <= radius; i++) {
            generateRing(center, i);
        }
    }

    Cell *HexGrid::getCell(const Hex &h) {
        return &map[h];
    }

    void HexGrid::getRing(const Hex &center, int radius, std::vector<Hex> &out_results) const {
        if (radius < 0) {
            Logger::Log(LogLevel::ERROR, "[HexGrid] Get Ring search radius must be non negative");
            return;
        }

        if (radius == 0) {
            out_results.push_back(center);
            return;
        }

        // Top Left corner. This part highly correlated with ALL_DIRECTIONS order.
        Hex tempHex = center + (Hex{0, -1, 1} * radius);

        for (HexDirection dir: ALL_DIRECTIONS) {
            for (int j = 0; j < radius; j++) {
                out_results.push_back(tempHex);
                tempHex = Hex::get_neighbor(tempHex, dir);
            }
        }
    }

    void HexGrid::getSpiral(const Hex &center, int radius, std::vector<Hex> &out_results) const {
        for (int i = 0; i <= radius; i++) {
            getRing(center, i, out_results);
        }
    }

    std::unordered_map<Hex, Cell, HexHash> &HexGrid::getMap() {
        return map;
    }
}
