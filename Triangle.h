//
//
//

#ifndef RAY_TRACER_TRIANGLE_H
#define RAY_TRACER_TRIANGLE_H

#include <sstream>

class Triangle : public Shape {
public:
    Vector3 vertex0, vertex1, vertex2; // Vertices of the triangle

    // Vertices should be specified in counter-clockwise order
    Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Material& material)
            : Shape(material, ShapeType::Triangle), vertex0(v0), vertex1(v1), vertex2(v2) {}

    // Intersection method: checks if a ray intersects the triangle
    bool intersect(const Ray& ray, float& t) const override {
        const float EPSILON = 1e-5f; // Custom epsilon value

        // Compute the triangle's normal
        Vector3 normal = (vertex2 - vertex0).cross(vertex1 - vertex0).normalize();

        // Compute denominator in t calculation
        float denominator = normal.dot(ray.direction);
        if (std::abs(denominator) < EPSILON) return false; // Ray is parallel to the plane
        t = (vertex0.dot(normal) - ray.origin.dot(normal)) / denominator;
        if (t < 0) return false;

        // Compute intersection point P
        Vector3 P = ray.origin + ray.direction * t;

        // Compute barycentric coordinates
        Vector3 v0 = vertex1 - vertex0, v1 = vertex2 - vertex0, v2 = P - vertex0;
        float d00 = v0.dot(v0);
        float d01 = v0.dot(v1);
        float d11 = v1.dot(v1);
        float d20 = v2.dot(v0);
        float d21 = v2.dot(v1);
        float denom = d00 * d11 - d01 * d01;
        float beta = (d11 * d20 - d01 * d21) / denom;
        float gamma = (d00 * d21 - d01 * d20) / denom;
        float alpha = 1.0f - beta - gamma;

        // Check if point is inside triangle
        if (alpha > -EPSILON && beta > -EPSILON && gamma > -EPSILON && alpha + beta + gamma <= 1 + EPSILON)
            return true;

        return false;
    }

    Vector3 normalAt(const Vector3& point) const override {
        // For a flat triangle, the normal is the same across the entire surface.
        // It's the cross product of two edges of the triangle.
        Vector3 edge1 = vertex1 - vertex0;
        Vector3 edge2 = vertex2 - vertex0;
        Vector3 normal = edge1.cross(edge2).normalize();
        return normal;
    }

    std::string toString() const override {
        std::ostringstream oss;
        oss << "Triangle with vertices (" << vertex0.x << ", " << vertex0.y << ", " << vertex0.z << "), "
            << "(" << vertex1.x << ", " << vertex1.y << ", " << vertex1.z << "), and "
            << "(" << vertex2.x << ", " << vertex2.y << ", " << vertex2.z << ")\n"
            << Shape::toString(); // Call base class's toString() method

        return oss.str();
    }

};

bool operator==(const Triangle& lhs, const Triangle& rhs) {
    return static_cast<const Shape&>(lhs) == static_cast<const Shape&>(rhs) &&  // Compare Shape properties
           lhs.vertex0 == rhs.vertex0 &&
           lhs.vertex1 == rhs.vertex1 &&
           lhs.vertex2 == rhs.vertex2;
}

std::ostream& operator<<(std::ostream& os, const Triangle& triangle) {
    os << triangle.toString();
    return os;
}

#endif //RAY_TRACER_TRIANGLE_H
