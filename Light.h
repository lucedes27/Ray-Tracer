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
    Vector3 intensity; // Color intensity
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
    Type type;

    Light(Type type, const Vector3& directionOrPosition, const Vector3& intensity, float constant = 1.0f, float linear = 0.0f, float quadratic = 0.0f)
            : direction(directionOrPosition), intensity(intensity), constantAttenuation(constant), linearAttenuation(linear), quadraticAttenuation(quadratic), type(type) {
        if (type == Type::Directional) {
            direction = directionOrPosition.normalize();
        } else {
            position = directionOrPosition;
        }
    }
};



#endif //RAY_TRACER_LIGHT_H
