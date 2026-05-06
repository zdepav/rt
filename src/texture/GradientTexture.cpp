#include "GradientTexture.hpp"

GradientTexture::GradientTexture(
    const Vec3 from_color,
    const Vec3 to_color,
    const TilingMode htile,
    const TilingMode vtile
) : Texture(htile, vtile), _colors {
    GradientTexture::ColorStop(0.0f, from_color),
    GradientTexture::ColorStop(1.0f, to_color)
} { }

void GradientTexture::add_color(const float position, const Vec3 color) {
    int i = 0;
    while (i < _colors.size() && position > _colors[i].position) {
        ++i;
    }
    this->_colors.emplace(this->_colors.begin() + i, position, color);
}

Vec3 GradientTexture::get_color(const float t) const {
    if (t <= this->_colors[0].position) {
        return this->_colors[0].color;
    } else if (t >= this->_colors[this->_colors.size() - 1].position) {
        return this->_colors[this->_colors.size() - 1].color;
    }
    for (int i = 1; i < this->_colors.size(); ++i) {
        if (t <= this->_colors[i].position) {
            const GradientTexture::ColorStop& stop1 = this->_colors[i - 1];
            const GradientTexture::ColorStop& stop2 = this->_colors[i];
            return stop1.color.lerp(
                stop2.color,
                (t - stop1.position) / (stop2.position - stop1.position)
            );
        }
    }
    // following line will never be reached
    return this->_colors[this->_colors.size() - 1].color;
}
