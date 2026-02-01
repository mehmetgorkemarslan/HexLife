#include "Hex.h"
#include <stdexcept>
#include "Logger.h"

namespace mapGen {
    // Class constructor
    Hex::Hex(int q_in, int r_in, int s_in) {
        if (q_in + r_in + s_in != 0) {
            Logger::Log(LogLevel::ERROR, "[Hex] Sum of q,r,s values must be 0");
            throw std::invalid_argument("Invalid Hex arguments");
        }
        this->q = q_in;
        this->r = r_in;
        this->s = s_in;
    }

    int Hex::length(const Hex &h) {
        return (abs(h.q) + abs(h.r) + abs(h.s)) / 2;
    }

    int Hex::distance(const Hex &a, const Hex &b) {
        return length({a - b});
    }

    Hex Hex::get_neighbor(const Hex &h, HexDirection direction) {
        int dQ = 0, dR = 0, dS = 0;
        switch (direction) {
            case HexDirection::RIGHT:
                dQ = 1;
                dS = -1;
                break;
            case HexDirection::BOTTOM_RIGHT:
                dS = -1;
                dR = 1;
                break;
            case HexDirection::BOTTOM_LEFT:
                dR = 1;
                dQ = -1;
                break;
            case HexDirection::LEFT:
                dQ = -1;
                dS = 1;
                break;
            case HexDirection::TOP_LEFT:
                dS = 1;
                dR = -1;
                break;
            case HexDirection::TOP_RIGHT:
                dR = -1;
                dQ = 1;
                break;
        }
        return {h.q + dQ, h.r + dR, h.s + dS};
    }

    std::size_t HexHash::operator()(const Hex &h) const {
        auto h1 = std::hash<int>{}(h.q);
        auto h2 = std::hash<int>{}(h.r);
        auto h3 = std::hash<int>{}(h.s);


        h1 ^= h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);
        h1 ^= h3 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);

        return h1;
    }
}
