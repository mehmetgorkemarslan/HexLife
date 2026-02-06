#pragma once
#include <cstddef>
#include <cstdint>
#include "Common.h"

namespace mapGen {
    /**
       @brief Represents a hex in cube coordinates (q, r, s) where q + r + s == 0.
       @details Cube coordinates used for axial/hex math. Coordinates are integer
                step units. Distance and neighbor functions operate in hex steps.
    */
    struct Hex {
        int q;
        int r;
        int s;

        /**
           @brief Construct a Hex from cube coordinates.
           @param q_in q coordinate.
           @param r_in r coordinate.
           @param s_in s coordinate.
           @returns Constructed Hex.
        */
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
           @brief Compute the distance from the origin to the given hex.
           @param h Hex to measure.
           @returns Distance in hex steps from origin. 0 for origin
        */
        static int length(const Hex &h);

        /**
           @brief Compute the distance in steps between two hexes.
           @param a First hex.
           @param b Second hex.
           @returns Minimum number of hex steps between a and b.
        */
        static int distance(const Hex &a, const Hex &b);

        /**
           @brief Return the neighbor hex in the given direction.
           @param h Source hex.
           @param direction Direction to move to find the neighbor.
           @returns Neighbor hex one step away in given direction.
        */
        static Hex get_neighbor(const Hex &h, HexDirection direction);

        /**
           @brief Convert cube coordinates to 2D Cartesian coordinates for rendering/placement.
           @param h Source hex in cube coordinates.
           @returns 2D position using the same scale as the implementation (sqrt(3) and 3/2 factors).
        */
        static Vector2 toCartesian(const Hex &h);
    };


    /**
       @brief Hash functor for Hex to be used in unordered containers.
       @details Combines q, r, s into a single size_t value; suitable for hash-based maps/sets.
       @returns size_t hash of the hex coordinates.
    */
    struct HexHash {
        std::size_t operator()(const Hex &h) const;
    };
}
