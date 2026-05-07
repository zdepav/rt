#include "Cylinder.hpp"

#include <cmath>
#include "Circle.hpp"

Cylinder::Cylinder(
    const Material* material,
    const Vec3 center,
    const Vec3 orientation,
    const float half_length,
    const float radius
) : Shape(material),
    center(center),
    orientation(orientation.normalize()),
    half_length(half_length),
    radius(radius),
    radius_squared(radius * radius),
    s2v(0.5f / half_length),
    has_bottom(false),
    has_top(false) { }

bool quadratic_roots(float a, const float b, const float c, float &hit1, float &hit2) {
    a *= 2;
    float discriminant = b * b - 2.0f * a * c;
    if (discriminant < EPSILON) {
        // 1 or 0 hits, 1 hit would mean the ray is barely touching the surface - we ignore these
        return false;
    }
    discriminant = std::sqrt(discriminant);
    const float b_term = (feq(b, 0) ? discriminant : -discriminant) - b;
    if (hit1 > hit2) {
        hit1 = b_term / a;
        hit2 = 2.0f * c / b_term;
    } else {
        hit1 = 2.0f * c / b_term;
        hit2 = b_term / a;
    }
    return true;
}

bool Cylinder::intersect(const Ray& ray, Ray& reflected, float& best_distance) const {
    if (feq(ray.direction.dot(this->orientation), 1)) {
        return false;
    }
    const Vec3 ro2c = ray.origin - this->center;
    const float co_rd = this->orientation.dot(ray.direction);
    const float Ca_dot_Rl = this->orientation.dot(ro2c);
    float hit1, hit2;
    if (!quadratic_roots(
        1 - co_rd * co_rd,
        2 * (ray.direction.dot(ro2c) - co_rd * Ca_dot_Rl),
        ro2c.magnitude_squared() - Ca_dot_Rl * Ca_dot_Rl - this->radius_squared,
        hit1,
        hit2
    )) {
        return false;
    }
    Vec3 intersection = ray.origin + ray.direction * hit1;
    float s = this->orientation.dot(intersection - this->center);
    if (std::abs(s) > this->half_length) {
        intersection = ray.origin + ray.direction * hit2;
        s = this->orientation.dot(intersection - this->center);
        if (std::abs(s) > this->half_length) {
            return false;
        }
    }
    float distance = (intersection - ray.origin).magnitude();
    if (distance > best_distance) {
        return false;
    }
    best_distance = distance;
    reflected.origin = intersection;
    reflected.surface_normal = (intersection - this->center - s * this->orientation).normalize();
    reflected.direction = ray.direction.reflect(reflected.surface_normal);
    reflected.source = (void*)this;
    reflected.color = Vec3::UNIT;
    if (this->material->texture) {
        reflected.texcoord.x = 0; // TODO: implement
        reflected.texcoord.y = s * this->s2v + 0.5f;
    }
    return true;
}

void Cylinder::add_top(Scene& scene, const Material* material) {
    if (!this->has_top) {
        this->has_top = true;
        scene.shape<Circle>(
            material ? material : this->material,
            this->center + this->orientation * this->half_length,
            this->orientation,
            this->radius
        );
    }
}

void Cylinder::add_bottom(Scene& scene, const Material* material) {
    if (!this->has_bottom) {
        this->has_bottom = true;
        scene.shape<Circle>(
            material ? material : this->material,
            this->center - this->orientation * this->half_length,
            -this->orientation,
            this->radius
        );
    }
}

void Cylinder::close(Scene& scene, const Material* material) {
    this->add_top(scene, material);
    this->add_bottom(scene, material);
}
