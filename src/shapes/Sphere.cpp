#include "Sphere.hpp"

#include "../utils.hpp"
#include <cmath>

Sphere::Sphere(const Material* material, const Vec3 center, const float radius) :
    Shape(material), center(center), radius(radius), inv_radius(1.0f / radius) { }

bool Sphere::intersect(const Ray& ray, Ray& reflected, float& best_distance) const {
    Vec3 origin = (ray.origin - this->center) * this->inv_radius;
    float t = -origin.dot(ray.direction);
    float offset = t * t - origin.magnitude_squared() + 1.0f;
    if (offset < -EPSILON) {
        return false;
    }
    if (offset > EPSILON) {
        offset = std::sqrt(offset);
        t = std::min(t - offset, t + offset);
    }
    if (t <= 0 or t * this->radius > best_distance) {
        return false;
    }
    best_distance = t * this->radius;
    reflected.surface_normal = (origin + ray.direction * t).normalize();
    reflected.origin = reflected.surface_normal * this->radius + this->center;
    reflected.direction = ray.direction.reflect(reflected.surface_normal);
    reflected.source = (void*)this;
    reflected.color = Vec3::UNIT;
    if (this->material->texture) {
        reflected.texcoord.x = std::atan2(
            reflected.surface_normal.z,
            reflected.surface_normal.x
        ) * HALF_OVER_PI + 0.5f;
        const float xz_len = reflected.surface_normal.xz().magnitude();
        reflected.texcoord.y = 0.5f - std::atan2(reflected.surface_normal.y, xz_len) * ONE_OVER_PI;
    }
    return true;
}
