#ifndef TILINGMODE_HPP
#define TILINGMODE_HPP

enum TilingMode {
    TILING_MODE_NONE = 0,
    TILING_MODE_CLAMP = 1,
    TILING_MODE_REPEAT = 2,
    TILING_MODE_BOUNCE = 3
};

float apply_tiling_mode_clamp(float x, float bound = 1.0f);

float apply_tiling_mode_repeat(float x, float bound = 1.0f);

float apply_tiling_mode_bounce(float x, float bound = 1.0f);

float apply_tiling_mode(float x, TilingMode tiling_mode);

#endif //TILINGMODE_HPP
