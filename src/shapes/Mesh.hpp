#ifndef MESH_SHAPE_HPP
#define MESH_SHAPE_HPP

#include <memory>
#include <unordered_map>
#include <vector>
#include "../Vec3.hpp"
#include "Shape.hpp"

class Mesh : public Shape {

    std::string file_path;

    struct Data {

        struct MeshFace {
            int vertices[3];
            int uvs[3];
            int normals[3];

            MeshFace(int v1, int v2, int v3);

            MeshFace(int v1, int v2, int v3, int t1, int t2, int t3);

            MeshFace(int v1, int v2, int v3, int t1, int t2, int t3, int n1, int n2, int n3);
        };

        std::vector<Vec3> vertices;
        std::vector<Vec3> colors;
        std::vector<int> vertex_colors;
        std::vector<Vec2> uvs;
        std::vector<Vec3> normals;
        std::vector<MeshFace> faces;
        int ref_count;

        Data(const char* filename);
    }* data;

    bool intersect_face(
        const Ray& ray,
        Ray& reflected,
        float& best_distance,
        const Data::MeshFace& face
    ) const;

    static std::unordered_map<
        std::string,
        std::unique_ptr<Data>
    > data_cache;

    static Data* acquire_data(std::string& path);

public:

    Mesh(const Material* material, const char* filename);

    ~Mesh() override;

    bool intersect(const Ray& ray, Ray& reflected, float& best_distance) const override;
};

#endif // MESH_SHAPE_HPP
