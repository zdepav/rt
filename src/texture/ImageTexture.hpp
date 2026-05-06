#ifndef IMAGE_TEXTURE_HPP
#define IMAGE_TEXTURE_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include "../Image.hpp"
#include "../TilingMode.hpp"
#include "../Vec2.hpp"
#include "../Vec3.hpp"
#include "Texture.hpp"

class ImageTexture final : public Texture {

    std::string file_path;
    Image* image; // not owned by the texture as it can be shared by multiple textures

    Vec3 rgb2vec(size_t cx, size_t cy) const;

    // static

    class CountedImage {
    public:
        Image image;
        int ref_count;

        explicit CountedImage(const char* path) : image(path), ref_count(0) { }
    };

    static std::unordered_map<
        std::string,
        std::unique_ptr<ImageTexture::CountedImage>
    > image_cache;

    static Image* acquire_image(std::string& path);

    static bool pixel_coord(float& x, size_t size, TilingMode mode, size_t& c1, size_t& c2);

public:

    explicit ImageTexture(
        const char* filename,
        TilingMode htile = TILING_MODE_CLAMP,
        TilingMode vtile = TILING_MODE_CLAMP
    );

    ~ImageTexture() override;

    Vec3 get_pixel(Vec2 uv) override;
};

#endif // IMAGE_TEXTURE_HPP
