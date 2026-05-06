#ifndef VEC3_HPP
#define VEC3_HPP

#include <algorithm>
#include <cmath>
#include <ostream>

#include "Vec2.hpp"

#define VEC3_MAP(V, FUNC) Vec3(FUNC((V).x), FUNC((V).y), FUNC((V).z))

struct Vec3 {

    float x;
    float y;
    float z;

    // constructors

    explicit Vec3(
        const float x = 0.0f,
        const float y = 0.0f,
        const float z = 0.0f
    ) : x(x), y(y), z(z) { }

    explicit Vec3(const float x, const Vec2 yz) : x(x), y(yz.x), z(yz.y) { }

    explicit Vec3(const Vec2 xy, const float z) : x(xy.x), y(xy.y), z(z) { }

    // ReSharper disable once CppNonExplicitConvertingConstructor
    Vec3(const Vec2 xz) : x(xz.x), y(0.0f), z(xz.y) { }

    // methods

    float magnitude() const {
        return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    }

    float magnitude_squared() const {
        return this->x * this->x + this->y * this->y + this->z * this->z;
    }

    Vec3 lerp(const Vec3 other, const float ratio) const {
        return Vec3(
            this->x + ratio * (other.x - this->x),
            this->y + ratio * (other.y - this->y),
            this->z + ratio * (other.z - this->z)
        );
    }

    Vec3 abs() const { return VEC3_MAP(*this, std::abs); }

    Vec3 clamp(const float lower = 0.0f, const float upper = 1.0f) const {
        return Vec3(
            std::clamp(this->x, lower, upper),
            std::clamp(this->y, lower, upper),
            std::clamp(this->z, lower, upper)
        );
    }

    Vec3 floor() const { return VEC3_MAP(*this, std::floor); }

    Vec3 round() const { return VEC3_MAP(*this, std::round); }

    Vec3 ceil() const { return VEC3_MAP(*this, std::ceil); }

    Vec3 trunc() const { return VEC3_MAP(*this, std::trunc); }

    float sum() const { return this->x + this->y + this->z; }

    Vec3 normalize() const;

    Vec3 to_length(float length) const;

    float dot(const Vec3 other) const {
        return this->x * other.x + this->y * other.y + this->z * other.z;
    }

    Vec3 cross(const Vec3 other) const {
        return Vec3(
            this->y * other.z - this->z * other.y,
            this->z * other.x - this->x * other.z,
            this->x * other.y - this->y * other.x
        );
    }

    Vec3 reflect(Vec3 normal) const;

    Vec3 rotate_around(Vec3 axis, float angle) const;

    Vec3 perpendicular() const;

    float angle(Vec3 other) const;

    Vec3 project(Vec3 other) const;

    Vec2 xy() const { return Vec2(this->x, this->y); }
    Vec2 yx() const { return Vec2(this->y, this->x); }
    Vec2 xz() const { return Vec2(this->x, this->z); }
    Vec2 zx() const { return Vec2(this->z, this->x); }
    Vec2 yz() const { return Vec2(this->y, this->z); }
    Vec2 zy() const { return Vec2(this->z, this->y); }

    Vec3 xxx() const { return Vec3(this->x, this->x, this->x); }
    Vec3 xxy() const { return Vec3(this->x, this->x, this->y); }
    Vec3 xxz() const { return Vec3(this->x, this->x, this->z); }
    Vec3 xyx() const { return Vec3(this->x, this->y, this->x); }
    Vec3 xyy() const { return Vec3(this->x, this->y, this->y); }
    Vec3 xyz() const { return *this; }
    Vec3 xzx() const { return Vec3(this->x, this->z, this->x); }
    Vec3 xzy() const { return Vec3(this->x, this->z, this->y); }
    Vec3 xzz() const { return Vec3(this->x, this->z, this->z); }
    Vec3 yxx() const { return Vec3(this->y, this->x, this->x); }
    Vec3 yxy() const { return Vec3(this->y, this->x, this->y); }
    Vec3 yxz() const { return Vec3(this->y, this->x, this->z); }
    Vec3 yyx() const { return Vec3(this->y, this->y, this->x); }
    Vec3 yyy() const { return Vec3(this->y, this->y, this->y); }
    Vec3 yyz() const { return Vec3(this->y, this->y, this->z); }
    Vec3 yzx() const { return Vec3(this->y, this->z, this->x); }
    Vec3 yzy() const { return Vec3(this->y, this->z, this->y); }
    Vec3 yzz() const { return Vec3(this->y, this->z, this->z); }
    Vec3 zxx() const { return Vec3(this->z, this->x, this->x); }
    Vec3 zxy() const { return Vec3(this->z, this->x, this->y); }
    Vec3 zxz() const { return Vec3(this->z, this->x, this->z); }
    Vec3 zyx() const { return Vec3(this->z, this->y, this->x); }
    Vec3 zyy() const { return Vec3(this->z, this->y, this->y); }
    Vec3 zyz() const { return Vec3(this->z, this->y, this->z); }
    Vec3 zzx() const { return Vec3(this->z, this->z, this->x); }
    Vec3 zzy() const { return Vec3(this->z, this->z, this->y); }
    Vec3 zzz() const { return Vec3(this->z, this->z, this->z); }

    bool empty() const { return this->x == 0.0f && this->y == 0.0f && this->z == 0.0f; }

    // operators

    Vec3 operator+() const { return *this; }

    Vec3 operator-() const { return Vec3(-this->x, -this->y, -this->z); }

    Vec3 operator+(const Vec3& b) const {
        return Vec3(this->x + b.x, this->y + b.y, this->z + b.z);
    }

    Vec3 operator+(const float b) const { return Vec3(this->x + b, this->y + b, this->z + b); }

    Vec3 operator-(const Vec3& b) const {
        return Vec3(this->x - b.x, this->y - b.y, this->z - b.z);
    }

    Vec3 operator-(const float b) const { return Vec3(this->x - b, this->y - b, this->z - b); }

    Vec3 operator*(const Vec3& b) const {
        return Vec3(this->x * b.x, this->y * b.y, this->z * b.z);
    }

    Vec3 operator*(const float b) const { return Vec3(this->x * b, this->y * b, this->z * b); }

    Vec3 operator/(const Vec3& b) const {
        return Vec3(this->x / b.x, this->y / b.y, this->z / b.z);
    }

    Vec3 operator/(const float b) const { return *this * (1.0f / b); }

    Vec3 operator%(const Vec3& b) const {
        return Vec3(std::fmod(this->x, b.x), std::fmod(this->y, b.y), std::fmod(this->z, b.z));
    }

    Vec3 operator%(const float b) const {
        return Vec3(std::fmod(this->x, b), std::fmod(this->y, b), std::fmod(this->z, b));
    }

    Vec3& operator +=(const Vec3& b) {
        this->x += b.x;
        this->y += b.y;
        this->z += b.z;
        return *this;
    }

    Vec3& operator +=(const float b) {
        this->x += b;
        this->y += b;
        this->z += b;
        return *this;
    }

    Vec3& operator -=(const Vec3& b) {
        this->x -= b.x;
        this->y -= b.y;
        this->z -= b.z;
        return *this;
    }

    Vec3& operator -=(const float b) {
        this->x -= b;
        this->y -= b;
        this->z -= b;
        return *this;
    }

    Vec3& operator *=(const Vec3& b) {
        this->x *= b.x;
        this->y *= b.y;
        this->z *= b.z;
        return *this;
    }

    Vec3& operator *=(const float b) {
        this->x *= b;
        this->y *= b;
        this->z *= b;
        return *this;
    }

    Vec3& operator /=(const Vec3& b) {
        this->x /= b.x;
        this->y /= b.y;
        this->z /= b.z;
        return *this;
    }

    Vec3& operator /=(const float b) { return *this *= 1.0f / b; }

    Vec3& operator %=(const Vec3& b) { return *this = *this % b; }

    Vec3& operator %=(const float b) { return *this = *this % b; }

    bool operator==(const Vec3& b) const {
        return this->x == b.x && this->y == b.y && this->z == b.z;
    }

    bool operator!=(const Vec3& b) const { return !this->operator==(b); }

    bool operator<(const Vec3& b) const {
        return this->magnitude_squared() < b.magnitude_squared();
    }

    bool operator>(const Vec3& b) const {
        return this->magnitude_squared() > b.magnitude_squared();
    }

    bool operator<=(const Vec3& b) const {
        return this->magnitude_squared() <= b.magnitude_squared();
    }

    bool operator>=(const Vec3& b) const {
        return this->magnitude_squared() >= b.magnitude_squared();
    }

    float operator[](const int index) const {
        switch (index) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
            default: return 0.0f;
        }
    }

    // static

    static const Vec3 ORIGIN;
    static const Vec3 UNIT_X;
    static const Vec3 UNIT_Y;
    static const Vec3 UNIT_Z;
    static const Vec3 UNIT;
};

inline Vec3 operator+(const float a, const Vec3& b) {
    return Vec3(a + b.x, a + b.y, a + b.z);
}

inline Vec3 operator-(const float a, const Vec3& b) {
    return Vec3(a - b.x, a - b.y, a - b.z);
}

inline Vec3 operator*(const float a, const Vec3& b) {
    return Vec3(a * b.x, a * b.y, a * b.z);
}

inline Vec3 operator/(const float a, const Vec3& b) {
    return Vec3(a / b.x, a / b.y, a / b.z);
}

inline Vec3 operator%(const float a, const Vec3& b) {
    return Vec3(std::fmod(a, b.x), std::fmod(a, b.y), std::fmod(a, b.z));
}

inline std::ostream& operator<<(std::ostream& os, const Vec3& vec) {
    return os << "Vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
}

#endif //VEC3_HPP
