//
//
//

#ifndef RAY_TRACER_RAY_H
#define RAY_TRACER_RAY_H


class Ray {
public:
    Vector3 origin;
    Vector3 direction;   // Normalized direction of the ray

    Ray(const Vector3& origin, const Vector3& direction) : origin(origin), direction(direction.normalize()) {}
};



#endif //RAY_TRACER_RAY_H
