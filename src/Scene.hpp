#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <type_traits>
#include <vector>
#include "Camera.hpp"
#include "Image.hpp"
#include "shapes/Shape.hpp"
#include "utils.hpp"

class Scene {

    std::vector<std::unique_ptr<Shape>> shapes;

    Vec3 normal2light(Vec3 normal) const;

    Vec3 reflect(
        Ray ray,
        const Material* material,
        int max_reflections,
        int reflections_left,
        const float max_distance
    ) const;

    Vec3 raytrace(
        const Ray& ray,
        int max_reflections,
        int reflections_left,
        bool debug,
        float max_distance
    ) const;

public:

    Vec3 light_direction;
    Vec3 light_color;
    Vec3 ambient_light;
    Texture* skybox;

    Scene();

    std::unique_ptr<Image> render(
        const Camera& camera,
        int width,
        int height,
        int antialias = 1,
        int max_reflections = 5,
        bool debug = false
    ) const;

    template<typename T> T& shape(std::unique_ptr<T>&& shape) {
        static_assert(std::is_base_of<Shape, T>::value, "T must inherit from Shape");
        this->shapes.push_back(std::move(shape));
        return *dynamic_cast<T*>(this->shapes[this->shapes.size() - 1].get());
    }

    template<typename T> T& shape(int i) {
        static_assert(std::is_base_of<Shape, T>::value, "T must inherit from Shape");
        ASSERT(i >= 0 && i < this->shapes.size(), "Shape index out of bounds");
        return *dynamic_cast<T*>(this->shapes[i].get());
    }

    int shape_count() { return this->shapes.size(); }
};

#endif // SCENE_HPP
