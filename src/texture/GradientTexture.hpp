#ifndef GRADIENT_TEXTURE_HPP
#define GRADIENT_TEXTURE_HPP

#include <vector>

#include "../TilingMode.hpp"
#include "../Vec3.hpp"
#include "Texture.hpp"

class GradientTexture : public Texture {

    struct ColorStop {
        float position;
        Vec3 color;
    };

protected:

    std::vector<GradientTexture::ColorStop> _colors;

    GradientTexture(Vec3 from_color, Vec3 to_color, TilingMode htile, TilingMode vtile);

    ~GradientTexture() override { }

    Vec3 get_color(float t) const;

public:

    void add_color(float position, Vec3 color);
};

#endif // GRADIENT_TEXTURE_HPP
