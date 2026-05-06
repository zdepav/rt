#include "ImageTexture.hpp"

#include <cstdlib>
#include "../utils.hpp"

std::unordered_map<
    std::string,
    std::unique_ptr<ImageTexture::CountedImage>
> ImageTexture::image_cache = {};

Image* ImageTexture::acquire_image(std::string& path) {
    auto iterator = ImageTexture::image_cache.find(path);
    if (iterator == ImageTexture::image_cache.end()) {
        const auto pair = ImageTexture::image_cache.emplace(
            path,
            std::make_unique<ImageTexture::CountedImage>(path.c_str())
        );
        ASSERT(pair.second, "Failed to add image to cache");
        iterator = pair.first;
    }
    ++iterator->second->ref_count;
    return &iterator->second->image;
}

bool ImageTexture::pixel_coord(
    float& x,
    size_t size,
    TilingMode mode,
    size_t& c1,
    size_t& c2
) {
    if (mode == TILING_MODE_REPEAT) {
        x = apply_tiling_mode_repeat(x * size - 0.5f, size);
        c1 = (size_t)x;
        c2 = (c1 + 1) % size;
        x -= c1;
        return true;
    } else if (mode == TILING_MODE_BOUNCE) {
        x = apply_tiling_mode_bounce(x) * (size - 1);
        if (x >= size - 1) {
            c1 = c2 = size - 1;
            x = 0.0f;
            return true;
        }
        c1 = (size_t)x;
        c2 = (c1 + 1) % size;
        x -= c1;
        return true;
    } else if (x < 0.0f) {
        c1 = c2 = 0;
        x = 0.0f;
        return mode == TILING_MODE_CLAMP;
    } else if (x > 1.0f) {
        c1 = c2 = size - 1;
        x = 0.0f;
        return mode == TILING_MODE_CLAMP;
    } else {
        x *= size - 1;
        c1 = (size_t)x;
        c2 = c1 < size - 1 ? c1 + 1 : c1;
        x -= c1;
        return true;
    }
}

constexpr float _BYTE_2_FLOAT_SQR = 1.0f / (255.0f * 255.0f);

Vec3 ImageTexture::rgb2vec(size_t cx, size_t cy) const {
    const RGBA pixel = this->image->get_pixel(cx, cy);
    const float a = pixel.a * _BYTE_2_FLOAT_SQR;
    return Vec3(pixel.r * a, pixel.g * a, pixel.b * a);
}

ImageTexture::ImageTexture(
    const char* filename,
    const TilingMode htile,
    const TilingMode vtile
) : Texture(htile, vtile),
    file_path(rt_abs_path(filename)),
    image(ImageTexture::acquire_image(this->file_path)) { }

ImageTexture::~ImageTexture() {
    const auto iterator = ImageTexture::image_cache.find(this->file_path);
    ASSERT(iterator != ImageTexture::image_cache.end(), "Corrupted image cache");
    if (--iterator->second->ref_count == 0) {
        ImageTexture::image_cache.erase(iterator);
    }
}

Vec3 ImageTexture::get_pixel(Vec2 uv) {
    size_t cx1, cx2, cy1, cy2;
    if (!(
        ImageTexture::pixel_coord(uv.x, this->image->width(), this->_htile, cx1, cx2) &&
        ImageTexture::pixel_coord(uv.y, this->image->height(), this->_vtile, cy1, cy2)
    )) {
        return Vec3::ORIGIN;
    } else if (uv.x == 0.0f) {
        if (uv.y == 0.0f) {
            return this->rgb2vec(cx1, cy1);
        }
        return this->rgb2vec(cx1, cy1).lerp(this->rgb2vec(cx1, cy2), uv.y);
    } else if (uv.y == 0.0f) {
        return this->rgb2vec(cx1, cy1).lerp(this->rgb2vec(cx2, cy1), uv.x);
    }
    return this->rgb2vec(cx1, cy1).lerp(this->rgb2vec(cx2, cy1), uv.x).lerp(
        this->rgb2vec(cx1, cy2).lerp(this->rgb2vec(cx2, cy2), uv.x),
        uv.y
    );
}
