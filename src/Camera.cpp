#include "Camera.hpp"

#include <cmath>
#include "utils.hpp"

Camera::Camera(const Vec3 position, const Vec3 target, const float fov, const float far) :
    position(position),
    forward((target - position).normalize()),
    focus(1.0f / std::tan(fov * PI / 360.0f)),
    _far(far) {
    ASSERT(position != target, "Camera position and target must be different");
    ASSERT(fov < 180, "FOV must be less than 180 degrees");
    ASSERT(far > 0, "Far plane must be in front of the camera");
    if (std::abs(this->forward.x) < EPSILON and std::abs(this->forward.z) < EPSILON) {
        // edge case, camera points straight up or down
        this->up = Vec3::UNIT_X;
        this->right = Vec3::UNIT_Z;
    } else {
        this->right = Vec3(-this->forward.z, 0.0f, this->forward.x).normalize();
        this->up = this->right.cross(this->forward).normalize();
    }
}

void Camera::raycast(Ray& ray, const float screen_x, const float screen_y) const {
    // screen_x is in -1..1, screen_y is in -height/width..height/width
    ray.origin = this->position;
    ray.direction = (
        this->forward * this->focus + this->right * screen_x - this->up * screen_y
    ).normalize();
    ray.source = (void*)this;
}
