#ifndef PLANE_SHAPE_HPP
#define PLANE_SHAPE_HPP

#include "Shape.hpp"
#include "../Material.hpp"
#include "../Ray.hpp"
#include "../Vec3.hpp"

class Plane final : public Shape {

    const Vec3 origin;
    const Vec3 normal;

public:

    Plane(const Material* material, Vec3 origin, Vec3 normal);

    bool intersect(const Ray& ray, Ray& reflected, float& best_distance) const override;
};

#endif // PLANE_SHAPE_HPP
