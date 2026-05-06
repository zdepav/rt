#ifndef RADIAL_GREDIENT_TEXTURE_HPP
#define RADIAL_GREDIENT_TEXTURE_HPP

#include "../TilingMode.hpp"
#include "../Vec2.hpp"
#include "../Vec3.hpp"
#include "GradientTexture.hpp"

class RadialGradientTexture final : public GradientTexture {

    const Vec2 _center;
    const Vec2 _inv_radius; // 1/radius
    const TilingMode _mode;

public:

    RadialGradientTexture(
        Vec2 center,
        Vec2 radius,
        Vec3 center_color = Vec3::ORIGIN,
        Vec3 border_color = Vec3::UNIT,
        TilingMode mode = TILING_MODE_CLAMP,
        TilingMode htile = TILING_MODE_NONE,
        TilingMode vtile = TILING_MODE_NONE
    );

    Vec3 get_pixel(Vec2 uv) override;
};

#endif // RADIAL_GREDIENT_TEXTURE_HPP
