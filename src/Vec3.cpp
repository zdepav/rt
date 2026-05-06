#include "Vec3.hpp"

Vec3 Vec3::normalize() const {
    return *this * (1 / this->magnitude());
}

Vec3 Vec3::to_length(float length) const {
    return *this * (length / this->magnitude());
}

Vec3 Vec3::reflect(const Vec3 normal) const {
    // normal must be normalized
    return *this - 2 * this->dot(normal) * normal;
}

Vec3 Vec3::rotate_around(const Vec3 axis, const float angle) const {
    // Rotate around `axis` by `angle`. Right hand rule applies.
    const float r2 = axis.magnitude_squared();
    const float ct = std::cos(angle);
    const float st = std::sin(angle) / std::sqrt(r2);
    const float dt = this->dot(axis) * (1 - ct) / r2;
    return Vec3(
        axis.x * dt + this->x * ct + (axis.y * this->z - axis.z * this->y) * st,
        axis.y * dt + this->y * ct + (axis.z * this->x - axis.x * this->z) * st,
        axis.z * dt + this->z * ct + (axis.x * this->y - axis.y * this->x) * st
    );
}

Vec3 Vec3::perpendicular() const {
    Vec3 v = this->cross(UNIT_Z);
    if (v.empty()) {
        v = this->cross(UNIT_Y);
    }
    return this->cross(v);
}

float Vec3::angle(const Vec3 other) const {
    return std::acos(
        this->dot(other) / std::sqrt(this->magnitude_squared() * other.magnitude_squared())
    );
}

Vec3 Vec3::project(const Vec3 other) const {
    // Project `self` onto `other`, `other` must be normalized!
    return this->dot(other) * other;
}

const Vec3 Vec3::ORIGIN = Vec3(0.0f, 0.0f, 0.0f);
const Vec3 Vec3::UNIT_X = Vec3(1.0f, 0.0f, 0.0f);
const Vec3 Vec3::UNIT_Y = Vec3(0.0f, 1.0f, 0.0f);
const Vec3 Vec3::UNIT_Z = Vec3(0.0f, 0.0f, 1.0f);
const Vec3 Vec3::UNIT = Vec3(1.0f, 1.0f, 1.0f);
