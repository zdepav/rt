#include "DebugTexture.hpp"

#include <algorithm>
#include <cmath>

DebugTexture::DebugTexture(const Vec2 grid_size) :
    Texture(TILING_MODE_NONE, TILING_MODE_NONE), _grid_size(grid_size) { }

Vec3 DebugTexture::get_pixel(const Vec2 uv) {
    Vec2 gr = uv * this->_grid_size + 0.5f;
    gr = 2.0f - (VEC2_MAP(gr, apply_tiling_mode_repeat) - 0.5f).abs() * 100.0f;
    return Vec3(uv.x, uv.y, std::clamp(std::max(gr.x, gr.y), 0.0f, 1.0f));
}
