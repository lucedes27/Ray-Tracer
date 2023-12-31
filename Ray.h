//
//
//

#ifndef RAY_TRACER_RAY_H
#define RAY_TRACER_RAY_H

#include "Matrix4x4.h"

class Ray {
public:
    Vector3 origin;
    Vector3 direction;   // Normalized direction of the ray

    Ray(const Vector3& origin, const Vector3& direction) : origin(origin), direction(direction.normalize()) {}

    Ray transformedBy(const Matrix4x4& matrix) const {
        // Transform the origin as a point
        Vector3 transformedOrigin = matrix * origin;

        // Transform the direction as a vector (ignoring translations)
        Vector3 transformedDirection = matrix * (origin + direction) - transformedOrigin;

        return Ray(transformedOrigin, transformedDirection);
    }
};

bool operator==(const Ray& lhs, const Ray& rhs) {
    return lhs.origin == rhs.origin && lhs.direction == rhs.direction;
}

std::ostream& operator<<(std::ostream& os, const Ray& ray) {
    os << "Ray(Origin: " << ray.origin << ", Direction: " << ray.direction << ")";
    return os;
}

#endif //RAY_TRACER_RAY_H
