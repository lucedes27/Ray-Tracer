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

    Vector3 globalAmbient = Vector3(0.2, 0.2, 0.2); // Global ambient light
    float constantAttenuation = 1.0; // Constant attenuation factor
    float linearAttenuation = 0.0;   // Linear attenuation factor
    float quadraticAttenuation = 0.0; // Quadratic attenuation factor

    Scene(const Vector3& lookfrom, const Vector3& lookat, const Vector3& up, float fovy, int width, int height)
            : eyePosition(lookfrom), lookAt(lookat), up(up), fovy(fovy), width(width), height(height) {
        // Calculate w, u, v based on eyePosition, up vector, and center
        Vector3 w = (eyePosition - lookAt).normalize();
        Vector3 u = up.cross(w).normalize();
        Vector3 v = w.cross(u);

        // Calculate fovx based on fovy and the image aspect ratio
        fovx = 2 * atan(tan(fovy / 2) * (float)width / (float)height);

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
        // Compute w, u, v based on eyePosition, up vector, and center
        Vector3 center = topLeft + (topRight - topLeft) * (sample.x / width) + (bottomLeft - topLeft) * (sample.y / height);
        float i = sample.y;
        float j = sample.x;

        Vector3 a = eyePosition - center;
        Vector3 b = up;
        Vector3 w = a.normalize();
        Vector3 u = b.cross(w).normalize();
        Vector3 v = w.cross(u);

        // Compute alpha and beta based on FOV angles and sample
        float alpha = tan(fovx / 2) * (j - (width / 2)) / (width / 2);
        float beta = tan(fovy / 2) * ((height / 2) - i) / (height / 2);

        // Compute the ray's origin and direction
        Vector3 origin = eyePosition;
        Vector3 direction = origin + alpha * u + beta * v - w;
        direction.normalize();

        return Ray(origin, direction);
    }


    Intersection intersect(const Ray& ray) const {
        float closestT = std::numeric_limits<float>::max();
        Intersection closestIntersection;

        for (const auto& object : objects) {
            float currentT;
            if (object->intersect(ray, currentT) && currentT < closestT) {
                Vector3 point = ray.origin + ray.direction * currentT;
                Vector3 normal = object->normalAt(point);
                closestIntersection = Intersection(point, normal, object);
                closestT = currentT;
            }
        }

        return closestIntersection;
    }

    bool isShadowed(const Ray& shadowRay, const std::shared_ptr<Light>& light) const {
        float distanceToLight = (light->position - shadowRay.origin).length();
        for (const auto& object : objects) {
            float currentT = std::numeric_limits<float>::max();
            if (object->intersect(shadowRay, currentT) && currentT < distanceToLight) {
                return true; // There is an object between the point and the light
            }
        }
        return false; // No objects are blocking the light
    }

    float attenuation(const Vector3& point, const std::shared_ptr<Light>& light) const {
        float distance = (light->position - point).length();
        return constantAttenuation / (constantAttenuation + linearAttenuation * distance + quadraticAttenuation * distance * distance);
    }


};



#endif //RAY_TRACER_SCENE_H
