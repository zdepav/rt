#include "LinearGradientTexture.hpp"

LinearGradientTexture::LinearGradientTexture(
    const Vec2 from,
    const Vec2 to,
    const Vec3 from_color,
    const Vec3 to_color,
    const TilingMode mode,
    const TilingMode htile,
    const TilingMode vtile
) : GradientTexture(from_color, to_color, htile, vtile),
    _a(to.x - from.x),
    _b(to.y - from.y),
    _c(from.x * from.x + from.y * from.y - from.x * to.x - from.y * to.y),
    _d(1 / (this->_c - from.x * to.x - from.y * to.y + to.x * to.x + to.y * to.y)),
    _mode(mode) { }

Vec3 LinearGradientTexture::get_pixel(const Vec2 uv) {
    return this->get_color(
        apply_tiling_mode(
            this->_d * (
                this->_a * apply_tiling_mode(uv.x, this->_htile) +
                this->_b * apply_tiling_mode(uv.y, this->_vtile) +
                this->_c
            ),
            this->_mode
        )
    );
}
