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
    Vector3 point;          // Transformed point of intersection
    Vector3 normal;         // Transformed normal at the intersection
    Vector3 originalPoint;  // Original point of intersection (before any transformations)
    Vector3 originalNormal; // Original normal at the intersection (before any transformations)
    std::shared_ptr<Shape> object; // Pointer to the intersected object (mainly for debugging purposes)
    Material material;      // Material of the intersected object

    Intersection() : hit(false) {}
    Intersection(const Vector3& point, const Vector3& normal, const std::shared_ptr<Shape>& obj)
            : hit(true), point(point), normal(normal), originalPoint(point), originalNormal(normal), object(obj), material(obj->material) {}

    // Conversion to bool to check if an intersection occurred
    operator bool() const {
        return hit;
    }

    Vector3 getPoint() const {
        return point;
    }

    Vector3 getNormal() const {
        return normal;
    }

    Vector3 getOriginalPoint() const {
        return originalPoint;
    }

    Vector3 getOriginalNormal() const {
        return originalNormal;
    }

    std::shared_ptr<Shape> getObject() const {
        return object;
    }

    Material getMaterial() const {
        return material;
    }

};



#endif //RAY_TRACER_INTERSECTION_H
