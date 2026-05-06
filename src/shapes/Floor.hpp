#ifndef FLOOR_SHAPE_HPP
#define FLOOR_SHAPE_HPP

#include "../Material.hpp"
#include "../Ray.hpp"
#include "../Vec3.hpp"
#include "Shape.hpp"

class Floor : public Shape {

    const float y;

public:

    Floor(const Material* material, float y = 0);

    bool intersect(const Ray& ray, Ray& reflected, float& best_distance) const override;
};


#endif // FLOOR_SHAPE_HPP
