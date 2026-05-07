#include "Vec2.hpp"

Vec2 Vec2::normalize() const {
    if (*this == Vec2::ORIGIN) {
        return Vec2::UNIT_X;
    }
    return *this * (1 / this->magnitude());
}

Vec2 Vec2::reflect(const Vec2 normal) const {
    // normal must be normalized
    return *this - 2 * this->dot(normal) * normal;
}

Vec2 Vec2::rotate(const float angle) const {
    const float ct = std::cos(angle);
    const float st = std::sin(angle);
    return Vec2(this->x * ct + this->y * st, this->y * ct - this->x * st);
}

Vec2 Vec2::to_length(const float length) const {
    if (feq(length, 0)) {
        return Vec2::ORIGIN;
    } else if (*this == Vec2::ORIGIN) {
        return Vec2::UNIT_X * length;
    }
    return *this * (length / this->magnitude());
}

Vec2 Vec2::perpendicular() const {
    return Vec2(this->y, -this->x);
}

float Vec2::angle(const Vec2 other) const {
    float mag = this->magnitude_squared() * other.magnitude_squared();
    if (mag <= 0.0f) {
        return 0.0f;
    }
    return std::acos(this->dot(other) / std::sqrt(mag));
}

/** Project `this` onto `other` */
Vec2 Vec2::project(const Vec2 other) const {
    if (other == Vec2::ORIGIN) {
        return Vec2::ORIGIN;
    }
    return other * (this->dot(other) / other.magnitude_squared());
}

const Vec2 Vec2::ORIGIN = Vec2(0.0f, 0.0f);
const Vec2 Vec2::UNIT_X = Vec2(1.0f, 0.0f);
const Vec2 Vec2::UNIT_Y = Vec2(0.0f, 1.0f);
const Vec2 Vec2::UNIT = Vec2(1.0f, 1.0f);
