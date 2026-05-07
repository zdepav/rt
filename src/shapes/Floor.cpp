#include "Floor.hpp"

#include "../utils.hpp"

Floor::Floor(const Material* material, const float y) : Shape(material), y(y) { }

bool Floor::intersect(const Ray& ray, Ray& reflected, float& best_distance) const {
    if (feq(ray.direction.y, 0)) {
        return false;
    }
    const float t = (this->y - ray.origin.y) / ray.direction.y;
    if (t <= 0 || t > best_distance) {
        return false;
    }
    reflected.origin = ray.origin + t * ray.direction;
    reflected.surface_normal = ray.direction.y < 0 ? Vec3::UNIT_Y : -Vec3::UNIT_Y;
    reflected.direction = ray.direction.reflect(reflected.surface_normal);
    reflected.source = (void*)this;
    reflected.color = Vec3::UNIT;
    if (this->material->texture) {
        reflected.texcoord = reflected.origin.xz();
    }
    return true;
}
