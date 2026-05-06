#include "RadialGradientTexture.hpp"

RadialGradientTexture::RadialGradientTexture(
    const Vec2 center,
    const Vec2 radius,
    const Vec3 center_color,
    const Vec3 border_color,
    const TilingMode mode,
    const TilingMode htile,
    const TilingMode vtile
) : GradientTexture(center_color, border_color, htile, vtile),
    _center(center),
    _inv_radius(1 / radius),
    _mode(mode) { }

Vec3 RadialGradientTexture::get_pixel(const Vec2 uv) {
    return this->get_color(
        apply_tiling_mode(((uv - this->_center) * this->_inv_radius).magnitude(), this->_mode)
    );
}
