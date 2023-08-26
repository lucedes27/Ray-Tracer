//
//
//

#ifndef RAY_TRACER_SCENE_H
#define RAY_TRACER_SCENE_H

#include "Ray.h"
#include "Shape.h"
#include "Light.h"
#include "Intersection.h"

class Scene {
public:
    Vector3 eyePosition; // Look from
    Vector3 lookAt;      // Look at
    Vector3 up;          // Up vector
    float fovy;          // Field of view in y direction
    float fovx;          // Field of view in x direction
    int width, height;
    Vector3 topLeft, topRight, bottomLeft, bottomRight; // Corners of the virtual screen
    std::vector<std::shared_ptr<Shape>> objects; // List of objects in the scene
    std::vector<std::shared_ptr<Light>> lights; // List of lights in the scene

    float constantAttenuation = 1.0; // Constant attenuation factor
    float linearAttenuation = 0.0;   // Linear attenuation factor
    float quadraticAttenuation = 0.0; // Quadratic attenuation factor

    int maxRecursionDepth = 5;

    Scene() = default;

    Scene(const Vector3& lookfrom, const Vector3& lookat, const Vector3& up, float fovy, int width, int height)
            : eyePosition(lookfrom), lookAt(lookat), up(up), fovy(fovy), width(width), height(height) {

        Vector3 forward = (lookAt - eyePosition).normalize();
        Vector3 right = (up.cross(forward)).normalize();
        Vector3 actualUp = forward.cross(right);
        this->up = actualUp;

        // Calculate w, u, v based on eyePosition, up vector, and center
        Vector3 w = (eyePosition - lookAt).normalize();
        Vector3 u = up.cross(w).normalize();
        Vector3 v = w.cross(u);

        setFovX();

        // Calculate the corner points of the virtual screen
        float alpha = tan(fovx / 2);
        float beta = tan(fovy / 2);
        topLeft = eyePosition + u * (-alpha) + v * beta - w;
        topRight = eyePosition + u * alpha + v * beta - w;
        bottomLeft = eyePosition + u * (-alpha) + v * (-beta) - w;
        bottomRight = eyePosition + u * alpha + v * (-beta) - w;
    }


    void addObject(const std::shared_ptr<Shape>& object) {
        objects.push_back(object);
    }

    void addLight(const std::shared_ptr<Light>& light) {
        lights.push_back(light);
    }

    Ray createRay(const Vector3& sample) const {
        // Compute the point on the virtual screen
        Vector3 center = topLeft + (topRight - topLeft) * (sample.x / width) + (bottomLeft - topLeft) * (sample.y / height);

        // Compute the ray's origin and direction
        Vector3 origin = eyePosition;
        Vector3 direction = center - origin; // Direction from the eye to the point on the virtual screen
        direction.normalize();

        return Ray(origin, direction);
    }


    Intersection intersect(const Ray& ray) const {
        float closestT = std::numeric_limits<float>::max();
        Intersection closestIntersection;

        for (const auto& object : objects) {
            // Transform the ray into the object's local space
            Ray localRay = ray.transformedBy(object->getInverseTransform());

            float currentT;
            if (object->intersect(localRay, currentT)) {
                Vector3 localPoint = localRay.origin + localRay.direction * currentT;
                Vector3 localNormal = object->normalAt(localPoint);

                // Transform the intersection point back to world space
                Vector3 worldPoint = object->transform * localPoint;
                Vector3 worldNormal = object->transform.inverse().transpose() * localNormal;

                // Compute the distance t in world space
                float worldT = (worldPoint - ray.origin).length();

                if (worldT < closestT) {
                    closestIntersection = Intersection(worldPoint, worldNormal, object);
                    closestT = worldT;
                }
            }
        }

        return closestIntersection;
    }

    bool isShadowed(const Ray& shadowRay, const std::shared_ptr<Light>& light) const {
        float distanceToLight;
        if (light->type == Light::Type::Directional) {
            distanceToLight = std::numeric_limits<float>::infinity(); // Infinite distance for directional lights
        } else {
            distanceToLight = (light->position - shadowRay.origin).length(); // Finite distance for point lights
        }

        for (const auto& object : objects) {
            // Transform the shadow ray into the object's local space
            Ray localShadowRay = shadowRay.transformedBy(object->getInverseTransform());

            float currentT = std::numeric_limits<float>::max();
            if (object->intersect(localShadowRay, currentT)) {
                // Transform the intersection point back to world space
                Vector3 localPoint = localShadowRay.origin + localShadowRay.direction * currentT;
                Vector3 worldPoint = object->transform * localPoint;

                // Compute the distance t in world space
                float worldT = (worldPoint - shadowRay.origin).length();

                if (worldT < distanceToLight) {
                    return true; // There is an object between the point and the light
                }
            }
        }
        return false; // No objects are blocking the light
    }

    float attenuation(const Vector3& point, const std::shared_ptr<Light>& light) const {
        float distance = (light->position - point).length();
        return constantAttenuation / (constantAttenuation + linearAttenuation * distance + quadraticAttenuation * distance * distance);
    }

    void updateVirtualScreen() {
        // Calculate w, u, v based on eyePosition, up vector, and center
        Vector3 w = (eyePosition - lookAt).normalize();
        Vector3 u = up.cross(w).normalize();
        Vector3 v = w.cross(u);

        // Calculate the corner points of the virtual screen
        float alpha = tan(fovx / 2);
        float beta = tan(fovy / 2);
        topLeft = eyePosition + u * (-alpha) + v * beta - w;
        topRight = eyePosition + u * alpha + v * beta - w;
        bottomLeft = eyePosition + u * (-alpha) + v * (-beta) - w;
        bottomRight = eyePosition + u * alpha + v * (-beta) - w;
    }

    void setAttenuation(float constant, float linear, float quadratic) {
        constantAttenuation = constant;
        linearAttenuation = linear;
        quadraticAttenuation = quadratic;
    }

    void setMaxRecursionDepth(int depth) {
        maxRecursionDepth = depth;
    }

    void setEyePosition(const Vector3& position) {
        eyePosition = position;
    }

    void setLookAt(const Vector3& position) {
        lookAt = position;
    }

    void setUp(const Vector3& vector) {
        up = vector;
    }

    void setFov(float fov) {
        fovy = fov;
        setFovX();
    }

    void setFovX() {
        fovx = 2 * atan(tan(fovy / 2) * ((float)width / (float)height));
    }

    friend std::ostream& operator<<(std::ostream& os, const Scene& scene);

};

bool operator==(const Scene& lhs, const Scene& rhs) {
    // Compare basic attributes
    if (lhs.eyePosition != rhs.eyePosition) return false;
    if (lhs.lookAt != rhs.lookAt) return false;
    if (lhs.up != rhs.up) return false;
    if (lhs.fovy != rhs.fovy) return false;
    if (lhs.fovx != rhs.fovx) return false;
    if (lhs.width != rhs.width) return false;
    if (lhs.height != rhs.height) return false;
    if (lhs.topLeft != rhs.topLeft) return false;
    if (lhs.topRight != rhs.topRight) return false;
    if (lhs.bottomLeft != rhs.bottomLeft) return false;
    if (lhs.bottomRight != rhs.bottomRight) return false;
    if (lhs.constantAttenuation != rhs.constantAttenuation) return false;
    if (lhs.linearAttenuation != rhs.linearAttenuation) return false;
    if (lhs.quadraticAttenuation != rhs.quadraticAttenuation) return false;
    if (lhs.maxRecursionDepth != rhs.maxRecursionDepth) return false;

    // Compare objects in the scene
    if (lhs.objects.size() != rhs.objects.size()) return false;
    for (const auto& objL : lhs.objects) {
        bool found = false;
        for (const auto& objR : rhs.objects) {
            if (*objL == *objR) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }

    // Compare lights in the scene
    if (lhs.lights.size() != rhs.lights.size()) return false;
    for (const auto& lightL : lhs.lights) {
        bool found = false;
        for (const auto& lightR : rhs.lights) {
            if (*lightL == *lightR) {
                found = true;
                break;
            }
        }
        if (!found) return false;
    }

    return true;
}

std::ostream& operator<<(std::ostream& os, const Scene& scene) {
    os << "Scene Details:" << std::endl;
    os << "Eye Position: " << scene.eyePosition << std::endl;
    os << "Look At: " << scene.lookAt << std::endl;
    os << "Up Vector: " << scene.up << std::endl;
    os << "FOV Y: " << scene.fovy << std::endl;
    os << "FOV X: " << scene.fovx << std::endl;
    os << "Width: " << scene.width << ", Height: " << scene.height << std::endl;

    os << "Top Left: " << scene.topLeft << std::endl;
    os << "Top Right: " << scene.topRight << std::endl;
    os << "Bottom Left: " << scene.bottomLeft << std::endl;
    os << "Bottom Right: " << scene.bottomRight << std::endl;

    os << "Max Recursion Depth: " << scene.maxRecursionDepth << std::endl;

    // Attenuation details
    os << "Attenuation:" << std::endl;
    os << "  Constant: " << scene.constantAttenuation << std::endl;
    os << "  Linear: " << scene.linearAttenuation << std::endl;
    os << "  Quadratic: " << scene.quadraticAttenuation << std::endl;

    os << "Objects in Scene: " << scene.objects.size() << std::endl;
    for (const auto& object : scene.objects) {
        os << object->toString() << std::endl;
    }
    os << "Lights in Scene: " << scene.lights.size() << std::endl;
    for (const auto& light : scene.lights) {
        os << light->toString() << std::endl;
    }
    return os;
}

#endif //RAY_TRACER_SCENE_H
