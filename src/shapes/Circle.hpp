#ifndef CIRCLE_SHAPE_HPP
#define CIRCLE_SHAPE_HPP

#include "Shape.hpp"
#include "../Material.hpp"
#include "../Ray.hpp"
#include "../Vec3.hpp"

class Circle final : public Shape {

    const Vec3 center;
    const Vec3 normal;
    const Vec3 tex_x;
    const Vec3 tex_y;
    const float tex_ratio;
    const float radius;
    const float radius_squared;

public:

    Circle(const Material* material, Vec3 center, Vec3 normal, float radius);

    bool intersect(const Ray& ray, Ray& reflected, float& best_distance) const override;
};

#endif // CIRCLE_SHAPE_HPP
