#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "../Vec3.hpp"
#include "../Ray.hpp"
#include "../Material.hpp"

class Shape {
protected:

    explicit Shape(const Material* material);

public:

    virtual ~Shape() = default;

    const Material* material;

    virtual bool intersect(const Ray& ray, Ray& reflected, float& best_distance) const = 0;
};

#endif // SHAPE_HPP
