#include "AngularGradientTexture.hpp"

#include "../utils.hpp"

AngularGradientTexture::AngularGradientTexture(
    const Vec2 center,
    const float aspect_ratio,
    const Vec3 from_color,
    const Vec3 to_color,
    const TilingMode htile,
    const TilingMode vtile
) : GradientTexture(from_color, to_color, htile, vtile),
    _center(center),
    _aspect_ratio(Vec2(aspect_ratio, 1.0f)) { }

Vec3 AngularGradientTexture::get_pixel(Vec2 uv) {
    uv = (uv - this->_center) * this->_aspect_ratio;
    return this->get_color(
        apply_tiling_mode(0.5 - std::atan2(uv.y, -uv.x) * HALF_OVER_PI, TILING_MODE_REPEAT)
    );
}
