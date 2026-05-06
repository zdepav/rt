#ifndef RAY_HPP
#define RAY_HPP

#include "Vec3.hpp"

struct Ray {
    Vec3 origin;
    Vec3 direction;      // normalized
    Vec3 surface_normal; // normalized
    Vec2 texcoord;
    Vec3 color;
    void* source;        // only used for equality comparisons so type doesn't matter
};

#endif //RAY_HPP
