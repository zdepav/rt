#ifndef SPHERE_SHAPE_HPP
#define SPHERE_SHAPE_HPP

#include "Shape.hpp"
#include "../Vec3.hpp"
#include "../Ray.hpp"
#include "../Material.hpp"

class Sphere final : public Shape {

    const Vec3 center;
    const float radius;
    const float inv_radius;

public:

    Sphere(const Material* material, Vec3 center, float radius);

    bool intersect(const Ray& ray, Ray& reflected, float& best_distance) const override;
};

#endif // SPHERE_SHAPE_HPP
