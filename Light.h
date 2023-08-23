//
//
//

#ifndef RAY_TRACER_LIGHT_H
#define RAY_TRACER_LIGHT_H

#include <sstream>

class Light {
public:
    enum class Type {
        Directional,
        Point
    };

    Vector3 direction; // Normalized direction of the light (for directional)
    Vector3 position;  // Position of the light (for point)
    Vector3 color;     // Color intensity
    Type type;

    Light(Type type, const Vector3& directionOrPosition, const Vector3& color)
            : color(color), type(type) {
        if (type == Type::Directional) {
            direction = directionOrPosition.normalize();
        } else {
            position = directionOrPosition;
        }
    }

    std::string toString() const {
        std::ostringstream oss;
        if (type == Type::Directional) {
            oss << "Directional Light with direction (" << direction.x << ", " << direction.y << ", " << direction.z << ")";
        } else {
            oss << "Point Light with position (" << position.x << ", " << position.y << ", " << position.z << ")";
        }
        oss << " and color intensity (" << color.x << ", " << color.y << ", " << color.z << ")";
        return oss.str();
    }

};

bool operator==(const Light& lhs, const Light& rhs) {
    return lhs.type == rhs.type &&
           lhs.direction == rhs.direction &&
           lhs.position == rhs.position &&
           lhs.color == rhs.color;
}

#endif //RAY_TRACER_LIGHT_H
