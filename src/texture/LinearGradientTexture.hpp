#ifndef LINEAR_GRADIENT_TEXTURE_HPP
#define LINEAR_GRADIENT_TEXTURE_HPP

#include "../TilingMode.hpp"
#include "../Vec2.hpp"
#include "../Vec3.hpp"
#include "GradientTexture.hpp"

class LinearGradientTexture final : public GradientTexture {

    const float _a;
    const float _b;
    const float _c;
    const float _d;
    const TilingMode _mode;

public:

    LinearGradientTexture(
        Vec2 from,
        Vec2 to,
        Vec3 from_color = Vec3::ORIGIN,
        Vec3 to_color = Vec3::UNIT,
        TilingMode mode = TILING_MODE_CLAMP,
        TilingMode htile = TILING_MODE_NONE,
        TilingMode vtile = TILING_MODE_NONE
    );

    Vec3 get_pixel(Vec2 uv) override;
};

#endif // LINEAR_GRADIENT_TEXTURE_HPP
