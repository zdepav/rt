#include "Image.hpp"

#include <iostream>
#include <cmath>
#include <algorithm>
#include "utils.hpp"

#define ASSERT_SPNG(EXPR) if (int error; (error = (EXPR))) { \
    std::cerr << "ERROR: Could not load image (" << spng_strerror(error) << ")\n"; \
    exit(1); \
}

Image::Image(const size_t width, const size_t height) : _width(width), _height(height) {
    ASSERT(width > 0 && height > 0, "Image can't be empty");
    ASSERT(width <= 32'768 && height <= 32'768, "Image size can't be more than 32 768 pixels");
    this->_data = std::make_unique<RGBA[]>(width * height);
}

Image::Image(const size_t width, const size_t height, const RGBA color) : Image(width, height) {
    for (size_t pixels = width * height, i = 0; i < pixels; ++i) {
        this->_data[i] = color;
    }
}

Image::Image(const char* filename) {
    const auto ctx = spng_ctx_new(0);
    const auto file = fopen(filename, "rb");
    ASSERT(file, "Could not open file (" << filename << ')');
    spng_set_png_file(ctx, file);
    spng_ihdr ihdr;
    ASSERT_SPNG(spng_get_ihdr(ctx, &ihdr));
    this->_width = ihdr.width;
    this->_height = ihdr.height;
    size_t image_size;
    ASSERT_SPNG(spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &image_size));
    this->_data = std::make_unique<RGBA[]>(image_size >> 2);
    ASSERT_SPNG(spng_decode_image(ctx, this->_data.get(), image_size, SPNG_FMT_RGBA8, 0));
    spng_ctx_free(ctx);
    fclose(file);
}

size_t Image::width() const { return this->_width; }

size_t Image::height() const { return this->_height; }

RGBA Image::get_pixel(const size_t x, const size_t y) const {
    ASSERT(x < this->_width && y < this->_height, "Image coordinates out of bounds");
    return this->_data[y * this->_width + x];
}

void Image::set_pixel(const size_t x, const size_t y, const RGBA color) {
    ASSERT(x < this->_width && y < this->_height, "Image coordinates out of bounds");
    this->_data[y * this->_width + x] = color;
}

void Image::reduce(const size_t ratio) {
    ASSERT(ratio != 0, "Reduction ratio must be greater than 0");
    if (ratio == 1) {
        return;
    }
    size_t width = this->_width / ratio;
    if (this->_width % ratio != 0) {
        ++width;
    }
    size_t height = this->_height / ratio;
    if (this->_height % ratio != 0) {
        ++height;
    }
    auto new_data = std::make_unique<RGBA[]>(width * height);
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            float r = 0, g = 0, b = 0, a = 0;
            const size_t right = std::min(this->_width, x * ratio + ratio);
            const size_t bottom = std::min(this->_height, y * ratio + ratio);
            size_t divisor = 0;
            for (size_t ox = x * ratio; ox < right; ++ox) {
                for (size_t oy = y * ratio; oy < bottom; ++oy) {
                    const auto color = this->get_pixel(ox, oy);
                    r += color.r;
                    g += color.g;
                    b += color.b;
                    a += color.a;
                    ++divisor;
                }
            }
            new_data[y * width + x] = RGBA(
                (int)std::round(r / divisor),
                (int)std::round(g / divisor),
                (int)std::round(b / divisor),
                (int)std::round(a / divisor)
            );
        }
    }
    this->_data = std::move(new_data);
    this->_width = width;
    this->_height = height;
}

void Image::save(const char* filename) const {
    spng_ctx* ctx = spng_ctx_new(SPNG_CTX_ENCODER);
    const auto file = fopen(filename, "wb");
    ASSERT(file, "Could not open file (" << filename << ')');
    spng_set_png_file(ctx, file);
    spng_ihdr ihdr = {};
    ihdr.width = (uint32_t)this->_width;
    ihdr.height = (uint32_t)this->_height;
    ihdr.bit_depth = 8;
    ihdr.color_type = SPNG_COLOR_TYPE_TRUECOLOR_ALPHA;
    spng_set_ihdr(ctx, &ihdr);
    ASSERT_SPNG(
        spng_encode_image(
            ctx,
            this->_data.get(),
            this->_width * this->_height * 4,
            SPNG_FMT_PNG,
            SPNG_ENCODE_FINALIZE
        )
    );
    spng_ctx_free(ctx);
    fclose(file);
}

RGBA::RGBA(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a) :
    r(r), g(g), b(b), a(a) {}

RGBA::RGBA(const int r, const int g, const int b, const int a) :
    r((uint8_t)std::clamp<int>(r, 0, 255)),
    g((uint8_t)std::clamp<int>(g, 0, 255)),
    b((uint8_t)std::clamp<int>(b, 0, 255)),
    a((uint8_t)std::clamp<int>(a, 0, 255)) { }

RGBA::RGBA(const float r, const float g, const float b, const float a) :
    RGBA((int)(r * 255), (int)(g * 255), (int)(b * 255), (int)(a * 255)) { }
