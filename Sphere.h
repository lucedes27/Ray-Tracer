//
//
//

#ifndef RAY_TRACER_SPHERE_H
#define RAY_TRACER_SPHERE_H

#include "Shape.h"

#include <cmath>

class Sphere : public Shape {
public:
    Vector3 center;
    float radius;

    Sphere(const Vector3& center, float radius, const Material& material)
            : Shape(material), center(center), radius(radius) {}

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
        return (point - center).normalize(); // Normal at a point on a sphere
    }

};




#endif //RAY_TRACER_SPHERE_H
