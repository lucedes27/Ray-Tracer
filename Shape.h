//
//
//

#ifndef RAY_TRACER_SHAPE_H
#define RAY_TRACER_SHAPE_H

#include "Material.h"

class Shape {
public:
    Material material; // Material of the shape

    Shape(const Material& material) : material(material) {}

    virtual bool intersect(const Ray& ray, float& t) const = 0; // Pure virtual method

    virtual Vector3 normalAt(const Vector3& point) const = 0; // Pure virtual method to calculate the normal

    virtual ~Shape() = default; // Virtual destructor
};



#endif //RAY_TRACER_SHAPE_H
