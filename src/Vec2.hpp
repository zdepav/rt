#ifndef VEC2_HPP
#define VEC2_HPP

#include <algorithm>
#include <cmath>
#include <ostream>
#include "utils.hpp"

#define VEC2_MAP(V, FUNC) Vec2(FUNC((V).x), FUNC((V).y))

struct Vec2 {

    float x;
    float y;

    // constructors

    explicit Vec2(const float x = 0.0f, const float y = 0.0f) : x(x), y(y) { }

    // methods

    float magnitude() const {
        return std::sqrt(this->x * this->x + this->y * this->y);
    }

    float magnitude_squared() const {
        return this->x * this->x + this->y * this->y;
    }

    Vec2 abs() const { return VEC2_MAP(*this, std::abs); }

    Vec2 clamp(const float lower = 0.0f, const float upper = 1.0f) const {
        return Vec2(std::clamp(this->x, lower, upper), std::clamp(this->y, lower, upper));
    }

    Vec2 floor() const { return VEC2_MAP(*this, std::floor); }

    Vec2 round() const { return VEC2_MAP(*this, std::round); }

    Vec2 ceil() const { return VEC2_MAP(*this, std::ceil); }

    Vec2 trunc() const { return VEC2_MAP(*this, std::trunc); }

    float sum() const { return this->x + this->y; }

    Vec2 normalize() const;

    Vec2 to_length(float length) const;

    Vec2 lerp(const Vec2 other, const float ratio) const {
        return Vec2(
            this->x + ratio * (other.x - this->x),
            this->y + ratio * (other.y - this->y)
        );
    }

    float dot(const Vec2 other) const { return this->x * other.x + this->y * other.y; }

    Vec2 perpendicular() const;

    Vec2 reflect(Vec2 normal) const;

    Vec2 rotate(float angle) const;

    float angle(Vec2 other) const;

    Vec2 project(Vec2 other) const;

    Vec2 xy() const { return *this; }
    Vec2 yx() const { return Vec2(this->y, this->x); }
    Vec2 xx() const { return Vec2(this->x, this->x); }
    Vec2 yy() const { return Vec2(this->y, this->y); }

    bool empty() const { return this->x == 0.0f && this->y == 0.0f; }

    // operators

    Vec2 operator+() const { return *this; }

    Vec2 operator-() const { return Vec2(-this->x, -this->y); }

    Vec2 operator+(const Vec2& b) const { return Vec2(this->x + b.x, this->y + b.y); }

    Vec2 operator+(const float b) const { return Vec2(this->x + b, this->y + b); }

    Vec2 operator-(const Vec2& b) const { return Vec2(this->x - b.x, this->y - b.y); }

    Vec2 operator-(const float b) const { return Vec2(this->x - b, this->y - b); }

    Vec2 operator*(const Vec2& b) const { return Vec2(this->x * b.x, this->y * b.y); }

    Vec2 operator*(const float b) const { return Vec2(this->x * b, this->y * b); }

    Vec2 operator/(const Vec2& b) const { return Vec2(this->x / b.x, this->y / b.y); }

    Vec2 operator/(const float b) const { return *this * (1.0f / b); }

    Vec2 operator%(const Vec2& b) const {
        return Vec2(std::fmod(this->x, b.x), std::fmod(this->y, b.y));
    }

    Vec2 operator%(const float b) const {
        return Vec2(std::fmod(this->x, b), std::fmod(this->y, b));
    }

    Vec2& operator +=(const Vec2& b) {
        this->x += b.x;
        this->y += b.y;
        return *this;
    }

    Vec2& operator +=(const float b) {
        this->x += b;
        this->y += b;
        return *this;
    }

    Vec2& operator -=(const Vec2& b) {
        this->x -= b.x;
        this->y -= b.y;
        return *this;
    }

    Vec2& operator -=(const float b) {
        this->x -= b;
        this->y -= b;
        return *this;
    }

    Vec2& operator *=(const Vec2& b) {
        this->x *= b.x;
        this->y *= b.y;
        return *this;
    }

    Vec2& operator *=(const float b) {
        this->x *= b;
        this->y *= b;
        return *this;
    }

    Vec2& operator /=(const Vec2& b) {
        this->x /= b.x;
        this->y /= b.y;
        return *this;
    }

    Vec2& operator /=(const float b) { return *this *= 1.0f / b; }

    Vec2& operator %=(const Vec2& b) { return *this = *this % b; }

    Vec2& operator %=(const float b) { return *this = *this % b; }

    bool operator==(const Vec2& b) const { return feq(this->x, b.x) and feq(this->y, b.y); }

    bool operator!=(const Vec2& b) const { return !this->operator==(b); }

    bool operator<(const Vec2& b) const {
        return this->magnitude_squared() < b.magnitude_squared();
    }

    bool operator>(const Vec2& b) const {
        return this->magnitude_squared() > b.magnitude_squared();
    }

    bool operator<=(const Vec2& b) const {
        return this->magnitude_squared() <= b.magnitude_squared();
    }

    bool operator>=(const Vec2& b) const {
        return this->magnitude_squared() >= b.magnitude_squared();
    }

    float operator[](const int index) const {
        switch (index) {
            case 0: return x;
            case 1: return y;
            default: return 0.0f;
        }
    }

    // static

    static const Vec2 ORIGIN;
    static const Vec2 UNIT_X;
    static const Vec2 UNIT_Y;
    static const Vec2 UNIT;
};

inline Vec2 operator+(const float a, const Vec2& b) {
    return Vec2(a + b.x, a + b.y);
}

inline Vec2 operator-(const float a, const Vec2& b) {
    return Vec2(a - b.x, a - b.y);
}

inline Vec2 operator*(const float a, const Vec2& b) {
    return Vec2(a * b.x, a * b.y);
}

inline Vec2 operator/(const float a, const Vec2& b) {
    return Vec2(a / b.x, a / b.y);
}

inline Vec2 operator%(const float a, const Vec2& b) {
    return Vec2(std::fmod(a, b.x), std::fmod(a, b.y));
}

inline std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
    return os << "Vec2(" << vec.x << ", " << vec.y << ")";
}

#endif //VEC2_HPP
