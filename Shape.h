//
//
//

#ifndef RAY_TRACER_SHAPE_H
#define RAY_TRACER_SHAPE_H

#include "Material.h"
#include "Transform.h"

#include <sstream>

enum class ShapeType {
    Triangle,
    Sphere
};

class Shape {
public:
    Material material; // Material of the shape
    ShapeType type;

    Shape(const Material& material, ShapeType type) : material(material), type(type) {}

    virtual bool intersect(const Ray& ray, float& t) const = 0; // Pure virtual method

    virtual Vector3 normalAt(const Vector3& point) const = 0; // Pure virtual method to calculate the normal

    virtual std::string toString() const {
        return "Shape with material properties";
    }

    void setTransform(const Transform& t) {
        transform = t;
    }

    const Transform& getTransform() const {
        return transform;
    }

    Matrix4x4 getInverseTransform() const {
        return transform.getCurrentTransform().inverse();
    }

    virtual ~Shape() = default; // Virtual destructor

protected:
    Transform transform; // Transform of the shape
};



#endif //RAY_TRACER_SHAPE_H
