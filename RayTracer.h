//
//
//

#ifndef RAY_TRACER_RAYTRACER_H
#define RAY_TRACER_RAYTRACER_H

#include "Film.h"
#include "Scene.h"
#include "Sampler.h"
#include "Vector3.h"
#include "Ray.h"

class RayTracer {
public:
    void trace(const Scene& scene, Film& film) {
        Sampler sampler;
        for (int y = 0; y < scene.height; y++) {
            for (int x = 0; x < scene.width; x++) {
                Vector3 sample = sampler.getSample(x, y);
                Ray ray = scene.createRay(sample);
                float t;
                Vector3 color(0, 0, 0); // Default to black
                if (scene.intersect(ray, t)) {
                    color = Vector3(1, 0, 0); // Set to red if hit
                }
                film.addSample(x, y, color);
            }
        }
    }
};



#endif //RAY_TRACER_RAYTRACER_H
