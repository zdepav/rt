#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "../TilingMode.hpp"
#include "../Vec2.hpp"
#include "../Vec3.hpp"

class Texture {
protected:

    const TilingMode _htile;
    const TilingMode _vtile;

    virtual ~Texture() { }

    Texture(const TilingMode htile, const TilingMode vtile) : _htile(htile), _vtile(vtile) { }

public:

    virtual Vec3 get_pixel(Vec2 uv) = 0; // x and y are from 0..1
};

#endif //TEXTURE_HPP
