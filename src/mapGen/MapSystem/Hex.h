#pragma once
#include <cstddef>
#include <cstdint>

namespace mapGen {
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

    /**
     * @brief Represents a hex in cube coordinates (q, r, s) where q + r + s == 0.
     *
     * Provides basic arithmetic operators, equality, and utility functions such as
     * length (distance from origin), distance between two hexes, and neighbor lookup.
     */
    struct Hex {
        int q, r, s;

        Hex(int q_in, int r_in, int s_in);

        // Operator Overrides
        Hex operator+(const Hex &o) const {
            return {q + o.q, r + o.r, s + o.s};
        }

        Hex operator-(const Hex &o) const {
            return {q - o.q, r - o.r, s - o.s};
        }

        Hex operator*(int scalar) const {
            return {q * scalar, r * scalar, s * scalar};
        }

        bool operator==(const Hex &o) const {
            // Just 2 might be enough but +1 compression is not that much for prevent errors
            return (q == o.q && r == o.r && s == o.s);
        }

        // Mathematical functions
        /**
         * @brief Compute the distance from the origin to the given hex.
         *
         * Returns the number of steps from (0,0,0) to h.
         *
         * @param h Hex to measure.
         * @return Distance from origin (0 for origin).
         */
        static int length(const Hex &h);

        /**
         * @brief Compute the distance in steps between two hexes.
         *
         * @param a First hex.
         * @param b Second hex.
         * @return The hex distance between a and b.
         */
        static int distance(const Hex &a, const Hex &b);

        /**
         * @brief Return the neighbor hex in the given direction.
         *
         * @param h Source hex.
         * @param direction Direction to move to find the neighbor.
         * @return Neighbor hex at one step in the given direction.
         */
        static Hex get_neighbor(const Hex &h, HexDirection direction);
    };


    /**
     * @brief Hash functor for Hex to be used in unordered containers.
     *
     * Combines the q, r, s integer values into a single size_t hash.
     */
    struct HexHash {
        std::size_t operator()(const Hex &h) const;
    };
}
