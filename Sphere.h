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
    Material material;

    Sphere(const Vector3& center, float radius, const Material& material) : center(center), radius(radius), material(material) {}

    // Intersection method: checks if a ray intersects the sphere
    bool intersect(const Ray& ray, float& t) const override {
        std::cout << "Ray origin x: " << ray.origin.x << std::endl;
        std::cout << "Ray origin y: " << ray.origin.y << std::endl;
        std::cout << "Ray origin z: " << ray.origin.z << std::endl;
        std::cout << "Ray direction x: " << ray.direction.x << std::endl;
        std::cout << "Ray direction y: " << ray.direction.y << std::endl;
        std::cout << "Ray direction z: " << ray.direction.z << std::endl;
        std::cout << "Intersecting with sphere" << std::endl;
        Vector3 oc = ray.origin - center;
        float a = ray.direction.dot(ray.direction);
        float b = 2.0f * oc.dot(ray.direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;

        std::cout << "a: " << a << std::endl;
        std::cout << "b: " << b << std::endl;
        std::cout << "c: " << c << std::endl;
        std::cout << "Discriminant: " << discriminant << std::endl;
        if (discriminant >= 0) {
            float t1 = (-b - std::sqrt(discriminant)) / (2.0f * a);
            float t2 = (-b + std::sqrt(discriminant)) / (2.0f * a);

            std::cout << "t1: " << t1 << std::endl;
            std::cout << "t2: " << t2 << std::endl;

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
};




#endif //RAY_TRACER_SPHERE_H
