#include "MapVisualizer.h"
#include "Logger.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Config.h"

namespace mapGen {
    void MapVisualizer::DisplayAllGrid(HexGrid &grid) {
        DisplayDisc(grid, Hex{0, 0, 0}, grid._radius);
    }

    void MapVisualizer::DisplayDisc(HexGrid &grid, const Hex &center, int radius) {
        if (Hex::length(center) + radius > grid._radius) {
            // Display area is out of bound
            Logger::Log(LogLevel::ERROR, "[HexGrid] displayDisc try to print out of bounds");
            throw std::invalid_argument("displayDisc try to print out of bounds");
        }

        std::stringstream ss;

        // Radius is 0. Just add center and return
        if (radius == 0) {
            grid.getCell(center)->getRepresentation(ss);
            Logger::Log(LogLevel::INFO, ss.str());
            return;
        }

        // Top left corner. Start printing here
        Hex head = center + (Hex{0, -1, 1} * radius);

        // Print Upper half of grid. Origin included
        for (int row = 0; row <= radius; row++) {
            Hex member = head;

            addSpaces(ss, row, radius);

            for (int i = 0; i <= radius + row; i++) {
                grid.getCell(member)->getRepresentation(ss);
                member = Hex::get_neighbor(member, HexDirection::RIGHT);
            }
            head = Hex::get_neighbor(head, HexDirection::BOTTOM_LEFT);
        }
        // Now we go 1 right position.
        // Go 1 block right
        head = Hex::get_neighbor(head, HexDirection::RIGHT);

        for (int row = radius - 1; row >= 0; row--) {
            Hex member = head;

            addSpaces(ss, row, radius);

            for (int i = 0; i <= radius + row; i++) {
                grid.getCell(member)->getRepresentation(ss);
                member = Hex::get_neighbor(member, HexDirection::RIGHT);
            }
            head = Hex::get_neighbor(head, HexDirection::BOTTOM_RIGHT);
        }

        // Print output.
        Logger::Log(LogLevel::INFO, ss.str());
    }


    void MapVisualizer::addSpaces(std::stringstream &ss, int row, int radius) {
        ss << "\n";
        for (int space = 0; space < radius - row; space++) {
            ss << "    ";
        }
    }


    // -------------------------------------------------

    void MapVisualizer::exportToPNG(HexGrid &grid, const std::string &filename) {
        auto &map = grid.getMap();
        if (map.empty()) return;

        // Settings
        const float HEX_SIZE = 24.0f;
        const float SQRT3 = 1.7320508f;
        const float HEX_WIDTH = SQRT3 * HEX_SIZE;
        const float HEX_HEIGHT = 2.0f * HEX_SIZE;

        // Find borders
        float R = static_cast<float>(grid._radius);
        float min_q = -R, max_q = R;
        float min_r = -R, max_r = R;

        float minPixelX = HEX_SIZE * SQRT3 * (min_q + min_r / 2.0f);
        float maxPixelX = HEX_SIZE * SQRT3 * (max_q + max_r / 2.0f);
        float minPixelY = HEX_SIZE * 1.5f * min_r;
        float maxPixelY = HEX_SIZE * 1.5f * max_r;

        int canvasWidth = (int) (maxPixelX - minPixelX + 2.5f * HEX_WIDTH);
        int canvasHeight = (int) (maxPixelY - minPixelY + 2.5f * HEX_HEIGHT);

        float offsetX = -minPixelX + HEX_WIDTH * 1.25f;
        float offsetY = -minPixelY + HEX_HEIGHT * 1.25f;

        std::vector<unsigned char> image(canvasWidth * canvasHeight * 3, 0);
        // Background color
        auto bgc = Config::getInnerSection("visuals", "colors").at("background_color");
        unsigned char bgcR = bgc.at(0).get<unsigned char>();
        unsigned char bgcG = bgc.at(1).get<unsigned char>();
        unsigned char bgcB = bgc.at(2).get<unsigned char>();
        for (int i = 0; i < image.size(); i) {
            image[i++] = bgcR;
            image[i++] = bgcG;
            image[i++] = bgcB;
        }

        // Render Loop
        for (const auto &[hex, cell]: map) {
            float cx = (HEX_SIZE * SQRT3 * (hex.q + hex.r / 2.0f)) + offsetX;
            float cy = (HEX_SIZE * 1.5f * hex.r) + offsetY;

            Color c = getBiomeColor(cell.biome);
            // Draw Hexagon
            drawHexagon(image, canvasWidth, canvasHeight, cx, cy, HEX_SIZE, c);
        }

        // Save
        // I use stbi_image_write to get image.
        if (stbi_write_png(filename.c_str(), canvasWidth, canvasHeight, 3, image.data(), canvasWidth * 3)) {
            Logger::Log(LogLevel::INFO, "Map Saved: " + filename);
        } else {
            Logger::Log(LogLevel::ERROR, "Map won't save!");
        }
    }

    void MapVisualizer::drawHexagon(std::vector<unsigned char> &image, int imgWidth, int imgHeight,
                                    float cx, float cy, float size, Color color) {
        const float SQRT3 = 1.7320508f;
        float borderSize = 2.0f;
        // Bounding Box

        int width = (int) std::ceil(size * SQRT3);
        int height = (int) std::ceil(size * 2.0f);

        int startX = std::max(0, (int) (cx - width / 2.0f - 1.0f));
        int endX = std::min(imgWidth, (int) (cx + width / 2.0f + 1.0f));
        int startY = std::max(0, (int) (cy - size - 1.0f));
        int endY = std::min(imgHeight, (int) (cy + size + 1.0f));

        float sizeBias = size + 0.6f;

        int hex_border_darkness = Config::get<int>("visuals", "hex_border_darkness");
        for (int y = startY; y < endY; y++) {
            for (int x = startX; x < endX; x++) {
                float dx = std::abs(x - cx);
                float dy = std::abs(y - cy);

                bool insideWidth = dx <= sizeBias * SQRT3 * 0.5f;
                bool insideCorner = (dy + dx * 0.57735f) <= sizeBias;

                if (insideWidth && insideCorner) {
                    int index = (y * imgWidth + x) * 3;

                    float distToVertical = (size * SQRT3 * 0.5f) - dx;
                    float distToDiagonal = size - (dy + dx * 0.57735f);

                    if (distToVertical < borderSize || distToDiagonal < borderSize) {
                        image[index] = (unsigned char) (color.r - hex_border_darkness);
                        image[index + 1] = (unsigned char) (color.g - hex_border_darkness);
                        image[index + 2] = (unsigned char) (color.b - hex_border_darkness);
                    } else {
                        // Biome Color
                        image[index] = color.r;
                        image[index + 1] = color.g;
                        image[index + 2] = color.b;
                    }
                }
            }
        }
    }

    MapVisualizer::Color MapVisualizer::getBiomeColor(BiomeType type) {
        static auto colors = Config::getInnerSection("visuals", "colors");
        auto color = colors.at("error");
        // TODO: bu kısımı cacle. Her seferinde baştan bakmakla uğraşmasın
        //  şuan her hexagon için tek tek giriyo
        switch (type) {
            case BiomeType::POLAR: color = colors.at("polar");
                break;
            case BiomeType::PLAINS: color = colors.at("plains");
                break;
            case BiomeType::FOREST: color = colors.at("forest");
                break;
            case BiomeType::DESERT: color = colors.at("desert");
                break;
            case BiomeType::JUNGLE: color = colors.at("jungle");
                break;
        }
        return {color.at(0).get<unsigned char>(), color.at(1).get<unsigned char>(), color.at(2).get<unsigned char>()};
    }
}
