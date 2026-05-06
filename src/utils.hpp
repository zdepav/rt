#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>

#define ASSERT(EXPR, MSG) if (!(EXPR)) { \
    std::cerr << "ERROR: " << MSG << '\n'; \
    exit(1); \
}

constexpr float PI = 3.141592653589f;
constexpr float ONE_OVER_PI = 1.0f / PI;
constexpr float HALF_OVER_PI = 0.5f / PI;

constexpr float EPSILON = std::numeric_limits<float>::epsilon(); // 0.000001f;

float frandom();

std::string rt_abs_path(const char* path);

#endif // UTILS_HPP
