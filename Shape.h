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
    Matrix4x4 transform; // Transform of the shape

    Shape(const Material& material, ShapeType type) : material(material), type(type) {}

    virtual bool intersect(const Ray& ray, float& t) const = 0; // Pure virtual method

    virtual Vector3 normalAt(const Vector3& point) const = 0; // Pure virtual method to calculate the normal

    virtual std::string toString() const {
        std::ostringstream oss;
        oss << "- Material properties: " << material << ",\n";
        oss << "- Transform Matrix:\n" << transform; // Print the transform
        return oss.str();
    }

    void setTransform(const Matrix4x4& t) {
        // Only allow setTransform on Sphere objects
        if (type == ShapeType::Sphere) {
            transform = t;
        }
    }

    const Matrix4x4& getTransform() const {
        return transform;
    }

    Matrix4x4 getInverseTransform() const {
        return transform.inverse();
    }

    virtual ~Shape() = default; // Virtual destructor

    friend bool operator==(const Shape& lhs, const Shape& rhs);

};

bool operator==(const Shape& lhs, const Shape& rhs) {
    return lhs.material == rhs.material &&
           lhs.type == rhs.type &&
           lhs.transform == rhs.transform;
}

std::ostream& operator<<(std::ostream& os, const Shape& shape) {
    os << shape.toString();
    return os;
}

#endif //RAY_TRACER_SHAPE_H
