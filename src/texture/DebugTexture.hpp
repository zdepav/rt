#ifndef DEBUG_TEXTURE_HPP
#define DEBUG_TEXTURE_HPP

#include "Texture.hpp"

class DebugTexture final : public Texture {

    const Vec2 _grid_size;

public:

    explicit DebugTexture(Vec2 grid_size);

    Vec3 get_pixel(Vec2 uv) override;
};

#endif // DEBUG_TEXTURE_HPP
