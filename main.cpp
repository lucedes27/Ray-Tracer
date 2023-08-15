//
//
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "Vector3.h"

#include "RayTracer.h"
#include "Ray.h"
#include "Light.h"
#include "Material.h"

#include "Shape.h"
#include "Sphere.h"
#include "Triangle.h"

#include "Scene.h"
#include "Film.h"
#include "Sampler.h"


void renderBSOD() {
    // Define the scene parameters
    Vector3 eyePosition(0, 0, -5);
    Vector3 lookAt(0, 0, 0); // Center of the scene
    Vector3 up(0, 1, 0); // Up vector
    float fovy = 90; // Field of view in y direction
    int width = 100;
    int height = 100;

    // Create the Scene object
    Scene scene(eyePosition, lookAt, up, fovy, width, height);

    Film film(scene.width, scene.height);

    for (int y = 0; y < scene.height; y++) {
        for (int x = 0; x < scene.width; x++) {
            // Since we are not tracing rays, the color is black
            Vector3 color(0, 0, 0);
            film.addSample(x, y, color);
        }
    }

    film.writeImage("BSOD.png");
}

void renderSphere() {
    Vector3 eye(0, 0, 0);
    Vector3 lookAt(0, 0, -1); // Center of the scene
    Vector3 up(0, 1, 0); // Up vector
    float fovy = 90; // Field of view in y direction
    int width = 1000;
    int height = 1000;

    // Create the Scene object
    Scene scene(eye, lookAt, up, fovy, width, height);

    Material material(
            Vector3(1.0, 0.0, 0.0), // Diffuse color (r, g, b) - Red
            Vector3(1.0, 1.0, 1.0), // Specular color (r, g, b) - Shiny reflection
            100.0,                 // Shininess - High shininess for a shiny surface
            Vector3(0.0, 0.0, 0.0)  // Emission color (r, g, b)
    );

    // Create a shared pointer to a sphere and add it to the scene
    std::shared_ptr<Shape> sphere = std::make_shared<Sphere>(Vector3(0, 0, -2), 1, material);
    scene.addObject(sphere);

    // Create a point light and add it to the scene
    Vector3 lightPosition(1, 1, 0); // Position of the light
    Vector3 lightColor(1.0, 1.0, 1.0); // Color of the light (white)
    std::shared_ptr<Light> pointLight = std::make_shared<Light>(Light::Type::Point, lightPosition, lightColor);
    scene.addLight(pointLight);

    // Create a film object to store the rendered image
    Film film(width, height);

    // Create a ray tracer object
    RayTracer rayTracer;

    // Render the scene using the ray tracer
    rayTracer.trace(scene, film);

    // Save the rendered image to a file
    film.writeImage("sphere.png");
}

int main() {

    renderBSOD();

    renderSphere();




    return 0;
}

