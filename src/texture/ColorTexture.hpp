#ifndef COLOR_TEXTURE_HPP
#define COLOR_TEXTURE_HPP

#include "../Vec2.hpp"
#include "../Vec3.hpp"
#include "Texture.hpp"

class ColorTexture final : public Texture {

    const Vec3 _color;

public:

    explicit ColorTexture(Vec3 color = Vec3::UNIT);

    Vec3 get_pixel(Vec2 uv) override;
};

#endif // COLOR_TEXTURE_HPP
