#include "Scene.hpp"

#include <chrono>
#include <cmath>

Scene::Scene() :
    shapes(),
    light_direction(Vec3(0.0f, 0.0f, 1.0f)),
    light_color(Vec3(0.9f, 0.9f, 0.9f)),
    ambient_light(Vec3(0.1f, 0.1f, 0.1f)),
    skybox(nullptr) { }

Vec3 Scene::normal2light(const Vec3 normal) const {
    float light = std::max(0.0f, -normal.dot(this->light_direction));
    return (light * light) * this->light_color + this->ambient_light;
}

Vec3 Scene::reflect(
    Ray ray,
    const Material* material,
    const int max_reflections,
    const int reflections_left,
    const float max_distance
) const {
    material->randomize_ray(ray);
    return this->raytrace(ray, max_reflections, reflections_left - 1, false, max_distance);
}

Vec3 Scene::raytrace(
    const Ray& ray,
    const int max_reflections,
    const int reflections_left,
    const bool debug,
    const float max_distance
) const {
    Shape* nearest_shape = nullptr;
    Ray nearest_hit;
    float nearest_dist = max_distance;
    for (int i = 0; i < this->shapes.size(); ++i) {
        Shape* shape = this->shapes[i].get();
        if (ray.source != shape && shape->intersect(ray, nearest_hit, nearest_dist)) {
            nearest_shape = shape;
        }
    }
    if (nearest_shape == nullptr) {
        if (debug || this->skybox == nullptr) {
            return Vec3::ORIGIN;
        }
        float xz_len = Vec3(ray.direction.x, ray.direction.z).magnitude();
        return this->skybox->get_pixel(
            Vec2(
                std::atan2(ray.direction.z, ray.direction.x) * HALF_OVER_PI + 0.5f,
                0.5f - std::atan2(ray.direction.y, xz_len) * ONE_OVER_PI
            )
        ) * this->normal2light(-ray.direction) * 0.5f;
    } else if (debug) {
        return nearest_hit.direction * 0.5f + 0.5f;
    }
    const Material* mat = nearest_shape->material;
    Vec3 base = mat->color * nearest_hit.color;
    if (mat->texture) {
        base *= mat->texture->get_pixel(nearest_hit.texcoord);
    }
    if (mat->emissive) {
        return base;
    }
    base *= this->normal2light(nearest_hit.surface_normal);
    int reflected_rays = mat->reflected_rays();
    if (reflections_left == 0 or reflected_rays == 0) {
        return base;
    } else if (reflections_left < max_reflections or reflected_rays == 1) {
        return base.lerp(
            this->reflect(nearest_hit, mat, max_reflections, reflections_left, max_distance),
            mat->glossiness
        );
    }
    Vec3 color;
    for (int i = 0; i < reflected_rays; ++i) {
        color += this->reflect(nearest_hit, mat, max_reflections, reflections_left, max_distance);
    }
    return base.lerp(color / reflected_rays, mat->glossiness);
}

std::unique_ptr<Image> Scene::render(
    const Camera& camera,
    const int width,
    const int height,
    const int antialias,
    int max_reflections,
    const bool debug
) const {
    auto t1 = std::chrono::high_resolution_clock::now();
    ASSERT(width > 0 && height > 0 && antialias > 0, "Invalid render image size");
    if (max_reflections < 0) {
        max_reflections = 5;
    }
    auto image = std::make_unique<Image>(width * antialias, height * antialias);
    float aspect_ratio = height / (float)width;
    Ray ray;
    int last_percentage = 0;
    auto last_t = std::chrono::high_resolution_clock::now();
    for (int y = 0; y < image->height(); ++y) {
        int percentage = (int)(y * 100.0f / image->height());
        auto now = std::chrono::high_resolution_clock::now();
        if (percentage > last_percentage) {
            if (static_cast<std::chrono::duration<float>>(now - last_t).count() > 2.0f) {
                last_t = now;
                last_percentage = percentage;
                std::cout << percentage << "%\n";
            }
        }
        for (int x = 0; x < image->width(); ++x) {
            const float two_over_w = 2.0f / (float)image->width();
            camera.raycast(ray, x * two_over_w - 1.0f, y * two_over_w - aspect_ratio);
            Vec3 color = this->raytrace(ray, max_reflections, max_reflections, debug, camera.far());
            image->set_pixel(x, y, RGBA(color.x, color.y, color.z, 1.0f));
        }
    }
    if (antialias > 1) {
        image->reduce(antialias);
    }
    std::chrono::duration<float, std::milli> ms = std::chrono::high_resolution_clock::now() - t1;
    const float msf = ms.count();
    std::cout << "Image rendered in ";
    if (msf > 60000) {
        std::cout << (int)(msf / 60000.0f) << "m " << std::fmod(msf / 1000.0f, 60.0f) << "s\n";
    } else if (msf > 1000) {
        std::cout << (msf / 1000.0f) << "s\n";
    } else {
        std::cout << msf << "ms\n";
    }
    return image;
}
