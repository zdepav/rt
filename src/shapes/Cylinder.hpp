#ifndef CYLINDER_SHAPE_HPP
#define CYLINDER_SHAPE_HPP

#include "Shape.hpp"
#include "../Material.hpp"
#include "../Ray.hpp"
#include "../Scene.hpp"
#include "../Vec3.hpp"

class Cylinder final : public Shape {

    const Vec3 center;
    const Vec3 orientation;
    const float half_length;
    const float radius;
    const float radius_squared;
    const float s2v;
    bool has_bottom;
    bool has_top;

public:

    Cylinder(
        const Material* material,
        Vec3 center,
        Vec3 orientation,
        float half_length,
        float radius
    );

    bool intersect(const Ray& ray, Ray& reflected, float& best_distance) const override;

    void add_top(Scene& scene, const Material* material = nullptr);

    void add_bottom(Scene& scene, const Material* material = nullptr);

    void close(Scene& scene, const Material* material = nullptr);
};

#endif // CYLINDER_SHAPE_HPP
