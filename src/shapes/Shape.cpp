#include "Shape.hpp"

Shape::Shape(const Material* material) : material(material ? material : &Material::DEFAULT) { }
