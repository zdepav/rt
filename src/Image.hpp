#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <cstdint>
#include <memory>
#include <spng.h>
#include <string>

struct RGBA {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    RGBA() : r(0), g(0), b(0), a(0) {}

    explicit RGBA(uint8_t r, uint8_t g = 0, uint8_t b = 0, uint8_t a = 255);

    explicit RGBA(int r, int g = 0, int b = 0, int a = 255);

    explicit RGBA(float r, float g = 0.0f, float b = 0.0f, float a = 1.0f);
};

class Image final {

    size_t _width;

    size_t _height;

    std::unique_ptr<RGBA[]> _data;

public:

    Image(size_t width, size_t height);

    Image(size_t width, size_t height, RGBA color);

    explicit Image(const char* filename);

    size_t width() const;

    size_t height() const;

    RGBA get_pixel(size_t x, size_t y) const;

    void set_pixel(size_t x, size_t y, RGBA color);

    void reduce(size_t ratio);

    void save(const char* filename) const;
};

#endif // IMAGE_HPP
