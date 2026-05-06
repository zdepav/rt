#ifndef ANGULAR_GRADIENT_TEXTURE_HPP
#define ANGULAR_GRADIENT_TEXTURE_HPP

#include "../TilingMode.hpp"
#include "../Vec2.hpp"
#include "../Vec3.hpp"
#include "GradientTexture.hpp"

class AngularGradientTexture final : public GradientTexture {

    const Vec2 _center;
    const Vec2 _aspect_ratio; // Vec2(width/height, 1)

public:

    explicit AngularGradientTexture(
        Vec2 center,
        float aspect_ratio = 1,
        Vec3 from_color = Vec3::UNIT,
        Vec3 to_color = Vec3::UNIT,
        TilingMode htile = TILING_MODE_NONE,
        TilingMode vtile = TILING_MODE_NONE
    );

    Vec3 get_pixel(Vec2 uv) override;
};

#endif // ANGULAR_GRADIENT_TEXTURE_HPP
