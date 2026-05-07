#ifndef FLOOR_SHAPE_HPP
#define FLOOR_SHAPE_HPP

#include "Shape.hpp"
#include "../Material.hpp"
#include "../Ray.hpp"

class Floor : public Shape {

    const float y;

public:

    explicit Floor(const Material* material, float y = 0);

    bool intersect(const Ray& ray, Ray& reflected, float& best_distance) const override;
};


#endif // FLOOR_SHAPE_HPP
