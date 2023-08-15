//
//
//

#ifndef RAY_TRACER_LIGHT_H
#define RAY_TRACER_LIGHT_H


class Light {
public:
    enum class Type {
        Directional,
        Point
    };

    Vector3 direction; // Normalized direction of the light (for directional)
    Vector3 position;  // Position of the light (for point)
    Vector3 color; // Color intensity
    Type type;

    Light(Type type, const Vector3& directionOrPosition, const Vector3& color)
            : direction(directionOrPosition), color(color), type(type) {
        if (type == Type::Directional) {
            direction = directionOrPosition.normalize();
        } else {
            position = directionOrPosition;
        }
    }
};



#endif //RAY_TRACER_LIGHT_H
