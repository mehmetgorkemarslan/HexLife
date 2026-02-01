#include "HexGrid.h"
#include "Logger.h"

namespace mapGen {
    void HexGrid::generateRing(const Hex &center, int radius) {
        this->_radius = radius;
        if (radius <= 0) {
            Logger::Log(LogLevel::ERROR, "[HexGrid] Ring radius must be bigger than 0");
            throw std::invalid_argument("Ring radius must be bigger than 0");
        }

        if (radius == 1) {
            map[center] = Cell();
            // TODO: implement biome settings map addition can be done in a function
            return;
        }

        Hex tempHex = center + (Hex{0, -1, 1} * radius);


        for (auto dir: ALL_DIRECTIONS) {
            for (int step = 0; step < radius; step++) {
                map[tempHex] = Cell();
                // TODO: implement biome settings, map addition can be done in a function
                tempHex = Hex::get_neighbor(tempHex, dir);
            }
        }
    }

    void HexGrid::addSpaces(std::stringstream &ss, int row, int radius) {
        ss << "\n";
        for (int space = 0; space < radius - row - 1; space++) {
            ss << "  ";
        }
    }

    void HexGrid::generateWorld(int radius) {
        if (radius <= 0) {
            Logger::Log(LogLevel::ERROR, "[HexGrid] World radius must be bigger than 0");
            throw std::invalid_argument("World radius must be bigger than 0");
        }
        Hex center = {0, 0, 0};

        for (int i = 1; i <= radius; i++) {
            generateRing(center, i);
        }
    }

    Cell *HexGrid::getCell(const Hex &h) {
        return &map[h];
    }

    void HexGrid::getRing(const Hex &center, int radius, std::vector<Hex> &out_results) const {
        if (radius <= 0) {
            Logger::Log(LogLevel::ERROR, "[HexGrid] Get Ring search radius is <= 0");
            return;
        }

        if (radius == 1) {
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
        for (int i = 1; i <= radius; i++) {
            getRing(center, i, out_results);
        }
    }

    void HexGrid::displayGrid() {
        displayDisc(Hex{0, 0, 0}, _radius);
    }

    void HexGrid::displayDisc(const Hex &center, int radius) {
        if (Hex::length(center) + radius > _radius) {
            // Display area is out of bound
            Logger::Log(LogLevel::ERROR, "[HexGrid] displayDisc try to print out of bounds");
            throw std::invalid_argument("displayDisc try to print out of bounds");
        }

        std::stringstream ss;

        // Radius is 1. Just add center and return
        if (radius == 1) {
            getCell(center)->getRepresentation(ss);
            Logger::Log(LogLevel::INFO, ss.str());
            return;
        }

        // Top left corner. Start printing here
        Hex head = center + (Hex{0, -1, 1} * radius);

        // Print Upper half of grid. Origin included
        for (int row = 0; row < radius; row++) {
            Hex member = head;

            addSpaces(ss, row, radius);

            for (int i = 0; i < radius + row; i++) {
                getCell(member)->getRepresentation(ss);
                member = Hex::get_neighbor(member, HexDirection::RIGHT);
            }
            head = Hex::get_neighbor(head, HexDirection::BOTTOM_LEFT);
        }

        // Now start bottom right for lower half of grid
        head = Hex::get_neighbor(head, HexDirection::BOTTOM_RIGHT);

        for (int row = radius - 2; row >= 0; row--) {
            Hex member = head;

            addSpaces(ss, row, radius);

            for (int i = 0; i < radius + row; i++) {
                getCell(member)->getRepresentation(ss);
                member = Hex::get_neighbor(member, HexDirection::RIGHT);
            }
            head = Hex::get_neighbor(head, HexDirection::BOTTOM_RIGHT);
        }

        // Print output.
        // TODO: Use a Buffered Logging
        Logger::Log(LogLevel::INFO, ss.str());
    }
}
