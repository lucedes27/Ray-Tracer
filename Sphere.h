//
//
//

#ifndef RAY_TRACER_SPHERE_H
#define RAY_TRACER_SPHERE_H

#include "Shape.h"

#include <cmath>
#include <sstream>

class Sphere : public Shape {
public:
    Vector3 center;
    float radius;

    Sphere(const Vector3& center, float radius, const Material& material)
            : Shape(material, ShapeType::Sphere), center(center), radius(radius) {}

    // Intersection method: checks if a ray intersects the sphere
    bool intersect(const Ray& ray, float& t) const override {
        Vector3 oc = ray.origin - center;
        float a = ray.direction.dot(ray.direction);
        float b = 2.0f * oc.dot(ray.direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant >= 0) {
            float t1 = (-b - std::sqrt(discriminant)) / (2.0f * a);
            float t2 = (-b + std::sqrt(discriminant)) / (2.0f * a);

            // If discriminant is zero, both roots are the same
            if (discriminant == 0) {
                t = t1; // or t = t2, since t1 = t2
                return true;
            }

            // Check if both roots are positive
            if (t1 > 0 && t2 > 0) {
                t = std::min(t1, t2);
                return true;
            }

            // Check if one root is positive
            if (t1 > 0) {
                t = t1;
                return true;
            }
            if (t2 > 0) {
                t = t2;
                return true;
            }
        }
        return false;
    }

    Vector3 normalAt(const Vector3& point) const override {
        // Calculate the normal in object space
        Vector3 objectNormal = (point - center).normalize();

        // Calculate the normal in world space
        Vector3 worldNormal = transform.inverse().transpose() * objectNormal;

        return worldNormal.normalize();
    }

    std::string toString() const override {
        std::ostringstream oss;
        oss << "Sphere with center (" << center.x << ", " << center.y << ", " << center.z << ") and radius " << radius
            << Shape::toString(); // Call base class's toString() method
        return oss.str();
    }

};

bool operator==(const Sphere& lhs, const Sphere& rhs) {
    return static_cast<const Shape&>(lhs) == static_cast<const Shape&>(rhs) &&  // Compare Shape properties
           lhs.center == rhs.center &&
           lhs.radius == rhs.radius;
}

std::ostream& operator<<(std::ostream& os, const Sphere& sphere) {
    os << sphere.toString();
    return os;
}

#endif //RAY_TRACER_SPHERE_H
