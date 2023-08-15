#ifndef RAY_TRACER_VECTOR3_H
#define RAY_TRACER_VECTOR3_H

#include <cmath>

class Vector3 {
public:
    float x, y, z;

    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    float dot(const Vector3& v) const { return x * v.x + y * v.y + z * v.z; }
    Vector3 cross(const Vector3& v) const { return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
    float length() const { return std::sqrt(x * x + y * y + z * z); }
    Vector3 normalize() const { return *this / length(); }
    Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); }
    friend Vector3 operator*(float scalar, const Vector3& v) { return Vector3(scalar * v.x, scalar * v.y, scalar * v.z); }
    Vector3 operator/(float scalar) const { return Vector3(x / scalar, y / scalar, z / scalar); }
    Vector3 operator-() const { return Vector3(-x, -y, -z); }
    Vector3& operator+=(const Vector3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    bool operator==(const Vector3& v) const {
        return x == v.x && y == v.y && z == v.z;
    }
    friend std::ostream& operator<<(std::ostream& os, const Vector3& v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }
    Vector3 operator*(const Vector3& v) const { return Vector3(x * v.x, y * v.y, z * v.z); }

    void clamp() {
        x = std::min(std::max(x, 0.0f), 1.0f);
        y = std::min(std::max(y, 0.0f), 1.0f);
        z = std::min(std::max(z, 0.0f), 1.0f);
    }
};


#endif //RAY_TRACER_VECTOR3_H
