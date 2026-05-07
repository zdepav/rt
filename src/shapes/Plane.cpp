#include "Plane.hpp"

#include <cmath>
#include "../utils.hpp"

Plane::Plane(const Material* material, const Vec3 origin, const Vec3 normal) :
    Shape(material), origin(origin), normal(normal.normalize()) { }

bool Plane::intersect(const Ray& ray, Ray& reflected, float& best_distance) const {
    const float rn = this->normal.dot(ray.direction);
    if (std::abs(rn) < EPSILON) {
        return false;
    }
    const float t = this->normal.dot(this->origin - ray.origin) / rn;
    if (t <= 0 || t > best_distance) {
        return false;
    }
    reflected.origin = ray.origin + t * ray.direction;
    reflected.surface_normal = rn < 0 ? normal : -normal;
    reflected.direction = ray.direction.reflect(reflected.surface_normal);
    reflected.source = (void*)this;
    reflected.color = Vec3::UNIT;
    if (this->material->texture) {
        // TODO: implement? will need an extra texture_scale field
        reflected.texcoord.x = 0;
        reflected.texcoord.y = 0;
    }
    return true;
}
