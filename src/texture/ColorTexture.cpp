#include "ColorTexture.hpp"

ColorTexture::ColorTexture(const Vec3 color) :
    Texture(TILING_MODE_NONE, TILING_MODE_NONE),
    _color(color) { }

Vec3 ColorTexture::get_pixel(const Vec2 uv) {
    return this->_color;
}
