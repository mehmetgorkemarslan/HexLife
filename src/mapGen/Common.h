#pragma once
#include <cstdint>

namespace mapGen {
    /**
     * @brief Biome classification for cells.
     *
     * These values provide a simple terrain categorization used for
     * visualization and simple behavior decisions during generation.
     */
    enum class BiomeType: uint8_t {
        POLAR,
        PLAINS,
        FOREST,
        DESERT,
        JUNGLE
    };

    struct Vector2 {
        float x;
        float y;
    };

    /**
     * @brief Directions for hex neighbors in cube coordinates.
     *
     * These values indicate the six primary neighboring directions for a hex cell.
     */
    enum class HexDirection: uint8_t {
        RIGHT,
        BOTTOM_RIGHT,
        BOTTOM_LEFT,
        LEFT,
        TOP_LEFT,
        TOP_RIGHT
    };

    /**
     * @note If iterating using a foreach-style loop, start from TOP_LEFT to preserve the expected ordering.
     */
    inline constexpr HexDirection ALL_DIRECTIONS[] = {
        HexDirection::RIGHT,
        HexDirection::BOTTOM_RIGHT,
        HexDirection::BOTTOM_LEFT,
        HexDirection::LEFT,
        HexDirection::TOP_LEFT,
        HexDirection::TOP_RIGHT
    };
}
