#include "Mesh.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "../utils.hpp"

Mesh::Data::MeshFace::MeshFace(const int v1, const int v2, const int v3) :
    vertices{v1 - 1, v2 - 1, v3 - 1},
    uvs{-1, -1, -1},
    normals{-1, -1, -1} { }

Mesh::Data::MeshFace::MeshFace(
    const int v1, const int v2, const int v3,
    const int t1, const int t2, const int t3
) :
    vertices{v1 - 1, v2 - 1, v3 - 1},
    uvs{t1 - 1, t2 - 1, t3 - 1},
    normals{-1, -1, -1} { }

Mesh::Data::MeshFace::MeshFace(
    const int v1, const int v2, const int v3,
    const int t1, const int t2, const int t3,
    const int n1, const int n2, const int n3
) :
    vertices{v1 - 1, v2 - 1, v3 - 1},
    uvs{t1 - 1, t2 - 1, t3 - 1},
    normals{n1 - 1, n2 - 1, n3 - 1} { }

Mesh::Data::Data(const char* filename) : colors{Vec3::UNIT} {
    // loading
    std::ifstream file(filename);
    ASSERT(file.is_open(), "Failed to load model");
    std::string line_str, buf;
    Vec3 vertex, color;
    int i[9];
    char ignore;
    while (std::getline(file, line_str)) {
        if (line_str.empty()) {
            continue;
        }
        std::stringstream line(line_str, std::ios::in);
        line >> buf;
        if (buf == "v") {
            line >> vertex.x >> vertex.y >> vertex.z >> color.x;
            this->vertices.push_back(vertex);
            if (line.eof()) {
                this->vertex_colors.push_back(0);
            } else {
                line >> color.y >> color.z;
                this->vertex_colors.push_back(this->colors.size());
                this->colors.push_back(color);
            }
        } else if (buf == "vt") {
            line >> vertex.x >> vertex.y;
            this->uvs.push_back(Vec2(vertex.x, 1.0f - vertex.y));
        } else if (buf == "vn") {
            line >> vertex.x >> vertex.y >> vertex.z;
            this->normals.push_back(vertex);
        } else if (buf == "f") {
            line >> i[0];
            if (line.peek() != '/') {
                // vertex only
                line >> i[1] >> i[2];
                this->faces.push_back(MeshFace(i[0], i[1], i[2]));
                continue;
            }
            line >> ignore;
            if (line.peek() == '/') {
                // vertex + normal
                line >> ignore >> i[6] >> i[1] >> ignore >> ignore >> i[7];
                line >> i[2] >> ignore >> ignore >> i[8];
                this->faces.push_back(MeshFace(i[0], i[1], i[2], 0, 0, 0, i[6], i[7], i[8]));
                continue;
            }
            line >> i[3];
            if (line.peek() == '/') {
                // vertex + uv + normal
                line >> ignore >> i[6] >> i[1] >> ignore >> i[4] >> ignore >> i[7];
                line >> i[2] >> ignore >> i[5] >> ignore >> i[8];
                this->faces.push_back(
                    MeshFace(i[0], i[1], i[2], i[3], i[4], i[5], i[6], i[7], i[8])
                );
                continue;
            }
            // vertex + uv
            line >> i[1] >> ignore >> i[4] >> i[2] >> ignore >> i[5];
            this->faces.push_back(MeshFace(i[0], i[1], i[2], i[3], i[4], i[5]));
        }
    }
    file.close();

    // validation
    ASSERT(!this->faces.empty(), "Failed to load model - no faces found");
    for (int j = 0; j < this->faces.size(); ++j) {
        MeshFace& face = this->faces[j];
        ASSERT(
            face.vertices[0] >= 0 && face.vertices[0] < vertices.size() &&
            face.vertices[1] >= 0 && face.vertices[1] < vertices.size() &&
            face.vertices[2] >= 0 && face.vertices[2] < vertices.size(),
            "Failed to load model - invalid vertex index"
        );
        ASSERT(
            (face.uvs[0] < 0 && face.uvs[1] < 0 && face.uvs[2] < 0) || (
                face.uvs[0] >= 0 && face.uvs[0] < uvs.size() &&
                face.uvs[1] >= 0 && face.uvs[1] < uvs.size() &&
                face.uvs[2] >= 0 && face.uvs[2] < uvs.size()
            ),
            "Failed to load model - invalid texture uv index"
        );
        ASSERT(
            (face.normals[0] < 0 && face.normals[1] < 0 && face.normals[2] < 0) || (
                face.normals[0] >= 0 && face.normals[0] < normals.size() &&
                face.normals[1] >= 0 && face.normals[1] < normals.size() &&
                face.normals[2] >= 0 && face.normals[2] < normals.size()
            ),
            "Failed to load model - invalid normal index"
        );
    }
}

std::unordered_map<std::string, std::unique_ptr<Mesh::Data>> Mesh::data_cache = {};

Mesh::Data* Mesh::acquire_data(std::string& path) {
    auto iterator = Mesh::data_cache.find(path);
    if (iterator == Mesh::data_cache.end()) {
        const auto pair = Mesh::data_cache.emplace(
            path,
            std::make_unique<Mesh::Data>(path.c_str())
        );
        ASSERT(pair.second, "Failed to add model data to cache");
        iterator = pair.first;
    }
    ++iterator->second->ref_count;
    return iterator->second.get();
}

Mesh::Mesh(const Material* material, const char* filename) :
    Shape(material),
    file_path(rt_abs_path(filename)),
    data(Mesh::acquire_data(this->file_path)) { }

Mesh::~Mesh() {
    const auto iterator = Mesh::data_cache.find(this->file_path);
    ASSERT(iterator != Mesh::data_cache.end(), "Corrupted model data cache");
    if (--iterator->second->ref_count == 0) {
        Mesh::data_cache.erase(iterator);
    }
}

bool Mesh::intersect(const Ray& ray, Ray& reflected, float& best_distance) const {
    bool success = false;
    for (int j = 0; j < this->data->faces.size(); ++j) {
        success |= this->intersect_face(ray, reflected, best_distance, this->data->faces[j]);
    }
    return success;
}

bool Mesh::intersect_face(
    const Ray& ray,
    Ray& reflected,
    float& best_distance,
    const Mesh::Data::MeshFace& face
) const {
    Vec3 a = this->data->vertices[face.vertices[0]];
    Vec3 b = this->data->vertices[face.vertices[1]] - a;
    Vec3 c = this->data->vertices[face.vertices[2]] - a;
    const Vec3 ray_cross_c = ray.direction.cross(c);
    const float det = ray_cross_c.dot(b);
    if (std::abs(det) < EPSILON) {
        return false;
    }
    const float inv_det = 1.0f / det;
    const Vec3 s = ray.origin - a;
    const float u = inv_det * s.dot(ray_cross_c);
    if (u < -EPSILON || (u > 1.0f)) {
        return false;
    }
    const Vec3 s_cross_b = s.cross(b);
    const float v = inv_det * ray.direction.dot(s_cross_b);
    if (v < -EPSILON || u + v > 1.0f) {
        return false;
    }
    const float t = inv_det * c.dot(s_cross_b);
    if (t < EPSILON or t > best_distance) {
        return false;
    }
    best_distance = t;
    reflected.origin = ray.origin + ray.direction * t;
    const float w = 1.0f - u - v;
    if (face.normals[0] < 0) {
        reflected.surface_normal = b.cross(c);
        if (reflected.surface_normal.dot(ray.direction) > 0.0f) {
            reflected.surface_normal = -reflected.surface_normal;
        }
    } else {
        reflected.surface_normal = (
            this->data->normals[face.normals[0]] * w +
            this->data->normals[face.normals[1]] * u +
            this->data->normals[face.normals[2]] * v
        ).normalize();
    }
    reflected.direction = ray.direction.reflect(reflected.surface_normal);
    reflected.source = (void*)this;
    reflected.color = (
        this->data->colors[this->data->vertex_colors[face.vertices[0]]] * w +
        this->data->colors[this->data->vertex_colors[face.vertices[1]]] * u +
        this->data->colors[this->data->vertex_colors[face.vertices[2]]] * v
    );
    if (this->material->texture) {
        if (face.uvs[0] < 0) {
            reflected.texcoord = Vec2::ORIGIN;
        } else {
            reflected.texcoord = (
                this->data->uvs[face.uvs[0]] * w +
                this->data->uvs[face.uvs[1]] * u +
                this->data->uvs[face.uvs[2]] * v
            );
        }
    }
    return true;
}
