//
//
//

#ifndef RAY_TRACER_INTERSECTION_H
#define RAY_TRACER_INTERSECTION_H

#include "Vector3.h"
#include "Material.h"

class Intersection {
public:
    bool hit;               // Whether an intersection occurred
    Vector3 point;          // Point of intersection
    Vector3 normal;         // Normal at the intersection
    Material material;      // Material of the intersected object

    Intersection() : hit(false) {}
    Intersection(const Vector3& point, const Vector3& normal, const Material& material)
            : hit(true), point(point), normal(normal), material(material) {}

    // Conversion to bool to check if an intersection occurred
    operator bool() const {
        return hit;
    }
};



#endif //RAY_TRACER_INTERSECTION_H
