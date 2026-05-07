#include "Material.hpp"

#include <cmath>
#include "utils.hpp"

constexpr int MAX_EXTRA_REFLECTION_RAYS = 7;

Material::Material(const Vec3 color, Texture* texture) :
    color(color), glossiness(0.0f), roughness(0.0f), texture(texture), emissive(false) { }

Material& Material::colored(const Vec3 color) {
    this->color = color;
    return *this;
}

Material& Material::glossy(const float glossiness) {
    this->glossiness = glossiness;
    return *this;
}

Material& Material::rough(const float roughness) {
    this->roughness = roughness;
    return *this;
}

Material& Material::textured(Texture* texture) {
    this->texture = texture;
    return *this;
}

Material& Material::emit(const bool emissive) {
    this->emissive = emissive;
    return *this;
}

bool Material::randomize_ray(Ray& ray) const {
    if (this->roughness == 0) {
        return false;
    }
    ray.direction = (
        ray.direction + Vec3(frandom() - 0.5, frandom() - 0.5, frandom() - 0.5) * this->roughness
    ).normalize();
    return true;
}

int Material::reflected_rays() const {
    if (this->glossiness == 0.0f) {
        return 0;
    }
    return 1 + (int)std::round(this->glossiness * MAX_EXTRA_REFLECTION_RAYS);
}

Material Material::DEFAULT = Material(Vec3::UNIT);
