#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Ray.hpp"
#include "Vec3.hpp"
#include "texture/Texture.hpp"

class Material {
public:

    Vec3 color;
    float glossiness;
    float roughness;
    Texture* texture;
    bool emissive;

    explicit Material(Vec3 color = Vec3::UNIT, Texture* texture = nullptr);

    Material& colored(Vec3 color);

    Material& glossy(float glossiness);

    Material& rough(float roughness);

    Material& textured(Texture* texture);

    Material& emit(bool emissive = true);

    bool randomize_ray(Ray& ray) const;

    int reflected_rays() const;

    static Material DEFAULT;
};

#endif //MATERIAL_HPP
