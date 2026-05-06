#include "TilingMode.hpp"

#include <cmath>

float apply_tiling_mode_clamp(const float x, const float bound) {
    return x < 0.0f ? 0.0f : x > bound ? bound : x;
}

float apply_tiling_mode_repeat(const float x, const float bound) {
    return std::fmod(x < 0 ? bound - std::fmod(-x, bound) : x, bound);
}

float apply_tiling_mode_bounce(const float x, const float bound) {
    return bound - bound * std::abs(1.0f - std::fmod(std::abs(x / bound), 2.0f));
}

float apply_tiling_mode(const float x, const TilingMode tiling_mode) {
    switch (tiling_mode) {
        case TILING_MODE_CLAMP:
            return x < 0.0f ? 0.0f : x > 1.0f ? 1.0f : x;
        case TILING_MODE_REPEAT:
            return std::fmod(x < 0 ? 1.0f - std::fmod(-x, 1.0f) : x, 1.0f);
        case TILING_MODE_BOUNCE:
            return 1.0f - std::abs(1.0f - std::fmod(std::abs(x), 2.0f));
        default:
            return x;
    }
}
