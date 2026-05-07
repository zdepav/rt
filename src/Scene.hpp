#ifndef SCENE_HPP
#define SCENE_HPP

#include <memory>
#include <type_traits>
#include <vector>
#include "Camera.hpp"
#include "Image.hpp"
#include "utils.hpp"
#include "shapes/Shape.hpp"

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

    /** Add a shape
     * @tparam T shape type
     * @tparam TArgs shape constructor argument types
     * @param args shape constructor arguments
     * @return reference to the added shape
     */
    template<typename T, typename... TArgs> T& shape(TArgs&&... args) {
        static_assert(std::is_base_of_v<Shape, T>, "T must inherit from Shape");
        this->shapes.push_back(std::make_unique<T>(std::forward<TArgs>(args)...));
        return *dynamic_cast<T*>(this->shapes[this->shapes.size() - 1].get());
    }

    /** Get an existing shape by index
     * @tparam T shape type
     * @param i index, `0 <= i < this->shape_count()`
     * @return reference to the shape
     */
    template<typename T> T& shape(const int i) {
        static_assert(std::is_base_of_v<Shape, T>, "T must inherit from Shape");
        ASSERT(i >= 0 && i < this->shapes.size(), "Shape index out of bounds");
        return *dynamic_cast<T*>(this->shapes[i].get());
    }

    int shape_count() const { return this->shapes.size(); }
};

#endif // SCENE_HPP
