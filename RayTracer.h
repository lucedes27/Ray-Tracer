//
//
//

#ifndef RAY_TRACER_RAYTRACER_H
#define RAY_TRACER_RAYTRACER_H

#include <algorithm>
#include <thread>
#include <vector>
#include <mutex>

#include "Film.h"
#include "Scene.h"
#include "Sampler.h"
#include "Vector3.h"
#include "Ray.h"

class RayTracer {
public:
    RayTracer(int maxRecursionDepth = 5) : maxRecursionDepth(maxRecursionDepth), pixelsProcessed(0) {}

    void trace(const Scene& scene, Film& film) {
        Sampler sampler;
        int totalPixels = scene.width * scene.height;
        int progressWidth = 50; // Width of the progress bar in characters
        const int progressBarUpdateFrequency = 100; // Update every 100 pixels

        auto startTime = std::chrono::high_resolution_clock::now();  // Record start time

        if (scene.maxRecursionDepth > 0) {
            maxRecursionDepth = scene.maxRecursionDepth;
        }

        // Parallelization stuff
        int numThreads = std::thread::hardware_concurrency(); // Get the number of available cores
        std::vector<std::thread> threads(numThreads);

        int rowsPerThread = scene.height / numThreads;

        for (int i = 0; i < numThreads; i++) {
            int startY = i * rowsPerThread;
            int endY = (i == numThreads - 1) ? scene.height : startY + rowsPerThread;

            threads[i] = std::thread([&, startY, endY]() {
                for (int y = startY; y < endY; y++) {
                    for (int x = 0; x < scene.width; x++) {
                        Vector3 sample = sampler.getSample(x, y);
                        Ray ray = scene.createRay(sample);
                        Intersection hit = scene.intersect(ray);
                        Vector3 color = findColor(ray, hit, scene);
                        film.addSample(x, y, color);

                        // Update progress bar
                        pixelsProcessed.fetch_add(1);
                        if (pixelsProcessed % progressBarUpdateFrequency == 0) {
                            std::lock_guard<std::mutex> lock(progressMutex);
                            int progress = (pixelsProcessed * progressWidth) / totalPixels;

                            auto currentTime = std::chrono::high_resolution_clock::now();
                            auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(
                                    currentTime - startTime).count();
                            float timePerPixel = elapsedTime / static_cast<float>(pixelsProcessed);
                            float estimatedRemainingTime = timePerPixel * (totalPixels - pixelsProcessed);

                            std::cout << "\r[";
                            for (int i = 0; i < progressWidth; i++) {
                                if (i < progress) {
                                    std::cout << "=";
                                } else {
                                    std::cout << " ";
                                }
                            }
                            std::cout << "] " << (100 * pixelsProcessed) / totalPixels
                                      << "%, Estimated time remaining: " << estimatedRemainingTime << "s";
                            std::cout.flush();
                        }
                    }
                }
            });
        }

        for (auto& thread : threads) {
            thread.join(); // Wait for all threads to finish
        }
        std::cout << "\n";
    }


private:
    int maxRecursionDepth;
    std::atomic<int> pixelsProcessed;
    std::mutex progressMutex;

    Vector3 findColor(const Ray& ray, const Intersection& intersection, const Scene& scene, int depth = 0) {
        if (!intersection) return Vector3(0, 0, 0); // Return black if no intersection

        Vector3 color = intersection.material.ambient + intersection.material.emission; // Global ambient and emission

        for (const auto& light : scene.lights) {
            Vector3 toLight;
            if (light->type == Light::Type::Directional) {
                toLight = -light->direction; // Directional light's direction is constant
            } else {
                toLight = (light->position - intersection.point).normalize(); // Point light's direction depends on position
            }
            Vector3 offset = toLight * 1e-3f; // Small offset towards the light
            Ray shadowRay(intersection.point + offset, toLight); // Start the shadow ray slightly towards the light

            // Check for shadow
            if (!scene.isShadowed(shadowRay, light)) {
                float attenuation = (light->type == Light::Type::Point) ? scene.attenuation(intersection.point, light) : 1.0f; // Apply attenuation only for point lights
                Vector3 diffuse = intersection.material.kd * std::max(0.0f, intersection.normal.dot(toLight));
                Vector3 viewDirection = -ray.direction; // View direction is opposite to ray direction
                Vector3 halfVector = (toLight + viewDirection).normalize(); // Half-vector
                Vector3 specular = intersection.material.ks * pow(std::max(0.0f, intersection.normal.dot(halfVector)), intersection.material.shininess);
                Vector3 lightContribution = (diffuse + specular) * light->color; // Multiply by the light's color intensity
                color += attenuation * lightContribution; // Apply attenuation
            }
        }

        // Reflection
        if (depth < maxRecursionDepth && !intersection.material.ks.isBlack()) {
            Vector3 reflectionDirection = ray.direction - 2 * ray.direction.dot(intersection.normal) * intersection.normal;
            Vector3 offset = reflectionDirection * 1e-3f; // Small offset in reflection direction
            Ray reflectionRay(intersection.point + offset, reflectionDirection);
            Vector3 reflectionColor = findColor(reflectionRay, scene.intersect(reflectionRay), scene, depth + 1);
            color += intersection.material.ks * reflectionColor; // Add reflection contribution
        }

        color.clamp();
        return color;
    }

};


#endif //RAY_TRACER_RAYTRACER_H
