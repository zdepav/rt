#ifndef TRIANGLE_SHAPE_HPP
#define TRIANGLE_SHAPE_HPP

#include "../Material.hpp"
#include "../Ray.hpp"
#include "../Vec2.hpp"
#include "../Vec3.hpp"
#include "Shape.hpp"

class Triangle : public Shape {

    const Vec3 vertices[3]; // B and C are stored as offsets from A
    const Vec3 colors[3];
    const Vec2 uvs[3];
    const Vec3 normal;

public:

    Triangle(const Material* material, Vec3 a, Vec3 b, Vec3 c);

    Triangle(
        const Material* material,
        Vec3 a, Vec3 b, Vec3 c,
        Vec3 a_color, Vec3 b_color, Vec3 c_color
    );

    Triangle(
        const Material* material,
        Vec3 a, Vec3 b, Vec3 c,
        Vec3 a_color, Vec3 b_color, Vec3 c_color,
        Vec2 a_uv, Vec2 b_uv, Vec2 c_uv
    );

    bool intersect(const Ray& ray, Ray& reflected, float& best_distance) const override;
};

#endif // TRIANGLE_SHAPE_HPP
