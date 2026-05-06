#include "Circle.hpp"

#include <cmath>
#include "../utils.hpp"

Circle::Circle(const Material* material, const Vec3 center, const Vec3 normal, const float radius) :
    Shape(material),
    center(center),
    normal(normal.normalize()),
    tex_x(this->normal.perpendicular().normalize()),
    tex_y(this->normal.cross(this->tex_x).normalize()),
    tex_ratio(0.5f / radius),
    radius(radius),
    radius_squared(radius * radius) { }

bool Circle::intersect(const Ray& ray, Ray& reflected, float& best_distance) const {
    const float rn = this->normal.dot(ray.direction);
    if (std::abs(rn) < EPSILON) {
        return false;
    }
    const float t = this->normal.dot(this->center - ray.origin) / rn;
    if (t <= 0 || t > best_distance) {
        return false;
    }
    Vec3 intersection = ray.origin + t * ray.direction;
    if ((intersection - this->center).magnitude_squared() > this->radius_squared) {
        return false;
    }
    best_distance = t;
    reflected.origin = intersection;
    reflected.surface_normal = rn < 0 ? normal : -normal;
    reflected.direction = ray.direction.reflect(reflected.surface_normal);
    reflected.source = (void*)this;
    reflected.color = Vec3::UNIT;
    if (this->material->texture) {
        intersection = intersection - this->center;
        // TODO: check correctness
        reflected.texcoord.x = intersection.project(this->tex_x).magnitude() * this->tex_ratio + 0.5;
        reflected.texcoord.y = intersection.project(this->tex_y).magnitude() * this->tex_ratio + 0.5;
    }
    return true;
}
