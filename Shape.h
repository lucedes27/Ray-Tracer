//
//
//

#ifndef RAY_TRACER_SHAPE_H
#define RAY_TRACER_SHAPE_H


class Shape {
public:
    virtual bool intersect(const Ray& ray, float& t) const = 0; // Pure virtual method

    virtual ~Shape() = default; // Virtual destructor

};


#endif //RAY_TRACER_SHAPE_H
