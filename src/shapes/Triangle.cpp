#include "Triangle.hpp"

#include <cmath>
#include "../utils.hpp"

Triangle::Triangle(const Material* material, Vec3 a, Vec3 b, Vec3 c) :
    Shape(material),
    vertices{a, b - a, c - a},
    colors{Vec3::UNIT, Vec3::UNIT, Vec3::UNIT},
    uvs{Vec2::ORIGIN, Vec2::ORIGIN, Vec2::ORIGIN},
    normal(this->vertices[1].cross(this->vertices[2]).normalize()) { }

Triangle::Triangle(
    const Material* material,
    Vec3 a, Vec3 b, Vec3 c,
    Vec3 a_color, Vec3 b_color, Vec3 c_color
) : Shape(material),
    vertices{a, b - a, c - a},
    colors{a_color, b_color, c_color},
    uvs{Vec2::ORIGIN, Vec2::ORIGIN, Vec2::ORIGIN},
    normal(this->vertices[1].cross(this->vertices[2]).normalize()) { }

Triangle::Triangle(
    const Material* material,
    Vec3 a, Vec3 b, Vec3 c,
    Vec3 a_color, Vec3 b_color, Vec3 c_color,
    Vec2 a_uv, Vec2 b_uv, Vec2 c_uv
) : Shape(material),
    vertices{a, b - a, c - a},
    colors{a_color, b_color, c_color},
    uvs{a_uv, b_uv, c_uv},
    normal(this->vertices[1].cross(this->vertices[2]).normalize()) { }

bool Triangle::intersect(const Ray& ray, Ray& reflected, float& best_distance) const {
    const Vec3 ray_cross_c = ray.direction.cross(this->vertices[2]);
    const float det = ray_cross_c.dot(this->vertices[1]);
    if (std::abs(det) < EPSILON) {
        return false;
    }
    const float inv_det = 1.0f / det;
    const Vec3 s = ray.origin - this->vertices[0];
    const float u = inv_det * s.dot(ray_cross_c);
    if (u < -EPSILON || (u > 1.0f)) {
        return false;
    }
    const Vec3 s_cross_b = s.cross(this->vertices[1]);
    const float v = inv_det * ray.direction.dot(s_cross_b);
    if (v < -EPSILON || u + v > 1.0f) {
        return false;
    }
    const float t = inv_det * this->vertices[2].dot(s_cross_b);
    if (t < EPSILON or t > best_distance) {
        return false;
    }
    best_distance = t;
    reflected.origin = ray.origin + ray.direction * t;
    reflected.surface_normal =
        this->normal.dot(ray.direction) < 0.0f ? this->normal : -this->normal;
    reflected.direction = ray.direction.reflect(reflected.surface_normal);
    reflected.source = (void*)this;
    reflected.color = this->colors[0] * (1.0f - u - v) + this->colors[1] * u + this->colors[2] * v;
    if (this->material->texture) {
        reflected.texcoord = this->uvs[0] * (1.0f - u - v) + this->uvs[1] * u + this->uvs[2] * v;
    }
    return true;
}