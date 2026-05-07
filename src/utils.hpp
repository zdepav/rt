#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

#define ASSERT(EXPR, MSG) if (!(EXPR)) { \
    std::cerr << "ERROR: " << MSG << '\n'; \
    exit(1); \
}

constexpr float PI = 3.141592653589f;
constexpr float ONE_OVER_PI = 1.0f / PI;
constexpr float HALF_OVER_PI = 0.5f / PI;

constexpr float EPSILON = 1e-7f;

float frandom();

/** Compare two numbers for (near) equality
 * @param a first number
 * @param b second number
 * @return whether `a` is equivalent to `b`
 */
inline bool feq(const float a, const float b) {
    return std::abs(a - b) < EPSILON;
}

std::string rt_abs_path(const char* path);

#endif // UTILS_HPP
