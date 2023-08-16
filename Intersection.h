//
//
//

#ifndef RAY_TRACER_INTERSECTION_H
#define RAY_TRACER_INTERSECTION_H

#include "Vector3.h"
#include "Material.h"
#include "Shape.h"

class Intersection {
public:
    bool hit;               // Whether an intersection occurred
    Vector3 point;          // Point of intersection
    Vector3 normal;         // Normal at the intersection
    std::shared_ptr<Shape> object; // Pointer to the intersected object (mainly for debugging purposes)
    Material material;      // Material of the intersected object

    Intersection() : hit(false) {}
    Intersection(const Vector3& point, const Vector3& normal, const std::shared_ptr<Shape>& obj)
            : hit(true), point(point), normal(normal), object(obj), material(obj->material) {}

    // Conversion to bool to check if an intersection occurred
    operator bool() const {
        return hit;
    }
};



#endif //RAY_TRACER_INTERSECTION_H
