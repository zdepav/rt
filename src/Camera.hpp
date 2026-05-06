#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <limits>
#include "Ray.hpp"
#include "Vec3.hpp"

class Camera {
    Vec3 position;
    Vec3 forward;
    Vec3 up;
    Vec3 right;
    float focus;
    float _far;

public:

    Camera(
        Vec3 position,
        Vec3 target,
        float fov = 60,
        float far = std::numeric_limits<float>::infinity()
    );

    void raycast(Ray& ray, float screen_x, float screen_y) const;

    float far() const { return this->_far; }
};

#endif // CAMERA_HPP
