//
//
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "Vector3.h"
#include "Matrix4x4.h"

#include "RayTracer.h"
#include "Ray.h"
#include "Light.h"
#include "Material.h"

#include "Shape.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Transform.h"

#include "Scene.h"
#include "Film.h"
#include "Sampler.h"

#include "Parser.h"

#include <tuple>


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

void renderTriangle() {
    Vector3 eye(0, 0, 0);
    Vector3 lookAt(0, 0, -1); // Center of the scene
    Vector3 up(0, 1, 0); // Up vector
    float fovy = 90; // Field of view in y direction
    int width = 1000;
    int height = 1000;

    // Create the Scene object
    Scene scene(eye, lookAt, up, fovy, width, height);

    Material material(
            Vector3(0.0, 1.0, 0.0), // Diffuse color (r, g, b) - Green
            Vector3(0.5, 0.5, 0.5), // Specular color (r, g, b) - Shiny reflection
            50.0,                 // Shininess - High shininess for a shiny surface
            Vector3(0.0, 0.0, 0.0)  // Emission color (r, g, b)
    );

    // Define triangle vertices
    Vector3 v0(-1, -1, -2);
    Vector3 v1(1, -1, -2);
    Vector3 v2(0, 1, -2);

    // Create a shared pointer to a triangle and add it to the scene
    std::shared_ptr<Shape> triangle = std::make_shared<Triangle>(v0, v1, v2, material);
    scene.addObject(triangle);

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
    film.writeImage("triangle.png");
}

void renderTransformedShapes() {
    Vector3 eye(0, 0, 0);
    Vector3 lookAt(0, 0, -1); // Center of the scene
    Vector3 up(0, 1, 0); // Up vector
    float fovy = 90; // Field of view in y direction
    int width = 1000;
    int height = 1000;

    // Create the Scene object
    Scene scene(eye, lookAt, up, fovy, width, height);

    Material material(
            Vector3(0.0, 1.0, 0.0), // Diffuse color (r, g, b) - Green
            Vector3(0.5, 0.5, 0.5), // Specular color (r, g, b) - Shiny reflection
            50.0,                 // Shininess - High shininess for a shiny surface
            Vector3(0.0, 0.0, 0.0)  // Emission color (r, g, b)
    );

    // Create a transform object
    Transform transform;

    // Apply transformations
    transform.translate(1, 1, 0);
    transform.rotate(0, 1, 0, 45); // Rotate 45 degrees around the y-axis
    transform.scale(0.5, 0.5, 0.5);

    // Define triangle vertices
    Vector3 v0(-1, -1, -2);
    Vector3 v1(1, -1, -2);
    Vector3 v2(0, 1, -2);

    // Apply the transformation to the triangle vertices
    v0 = transform.getCurrentTransform() * v0;
    v1 = transform.getCurrentTransform() * v1;
    v2 = transform.getCurrentTransform() * v2;

    // Create a shared pointer to a triangle and add it to the scene
    std::shared_ptr<Shape> triangle = std::make_shared<Triangle>(v0, v1, v2, material);
    scene.addObject(triangle);

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
    film.writeImage("transformedTriangle.png");
}

void renderComplexScene() {
    Vector3 eye(0, 0, 0);
    Vector3 lookAt(0, 0, -1); // Center of the scene
    Vector3 up(0, 1, 0); // Up vector
    float fovy = 90; // Field of view in y direction
    int width = 1000;
    int height = 1000;

    // Create the Scene object
    Scene scene(eye, lookAt, up, fovy, width, height);

    // Red sphere material
    Material redMaterial(
            Vector3(1.0, 0.0, 0.0),
            Vector3(1.0, 1.0, 1.0),
            100.0,
            Vector3(0.0, 0.0, 0.0)
    );

    // Green sphere material
    Material greenMaterial(
            Vector3(0.0, 1.0, 0.0),
            Vector3(1.0, 1.0, 1.0),
            100.0,
            Vector3(0.0, 0.0, 0.0)
    );

    // Blue triangle material
    Material blueMaterial(
            Vector3(0.0, 0.0, 1.0),
            Vector3(1.0, 1.0, 1.0),
            100.0,
            Vector3(0.0, 0.0, 0.0)
    );

    // Create spheres and add them to the scene
    std::shared_ptr<Shape> redSphere = std::make_shared<Sphere>(Vector3(0, 0, -2), 1, redMaterial);
    std::shared_ptr<Shape> greenSphere = std::make_shared<Sphere>(Vector3(2, 0, -3), 0.5, greenMaterial);
    scene.addObject(redSphere);
    scene.addObject(greenSphere);

    // Create a triangle and add it to the scene
    Vector3 v0(-2, -1, -3);
    Vector3 v1(-3, 1, -3);
    Vector3 v2(-1, 1, -3);
    std::shared_ptr<Shape> triangle = std::make_shared<Triangle>(v0, v1, v2, blueMaterial);
    scene.addObject(triangle);

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
    film.writeImage("complexScene.png");
}

void renderTriangleTest() {
    Vector3 eye(0, 0, 0);
    Vector3 lookAt(0, 0, -1); // Center of the scene
    Vector3 up(0, 1, 0); // Up vector
    float fovy = 90; // Field of view in y direction
    int width = 1000;
    int height = 1000;

    // Create the Scene object
    Scene scene(eye, lookAt, up, fovy, width, height);

    // Blue triangle material
    Material blueMaterial(
            Vector3(0.0, 0.0, 1.0),
            Vector3(1.0, 1.0, 1.0),
            100.0,
            Vector3(0.0, 0.0, 0.0)
    );

    // Define triangle vertices
    Vector3 v2(-2, -1, -3);
    Vector3 v1(-3, 1, -3);
    Vector3 v0(-1, 1, -3);

    // Create a triangle and add it to the scene
    std::shared_ptr<Shape> triangle = std::make_shared<Triangle>(v0, v1, v2, blueMaterial);
    scene.addObject(triangle);

    // Create a point light and add it to the scene
    Vector3 lightPosition(1, 1, 0); // Position of the light
    Vector3 lightColor(1.0, 1.0, 1.0); // Color of the light (white)
    std::shared_ptr<Light> pointLight = std::make_shared<Light>(Light::Type::Point, lightPosition, lightColor);
    scene.addLight(pointLight);

    std::cout << scene << std::endl;

    // Create a film object to store the rendered image
    Film film(width, height);

    // Create a ray tracer object
    RayTracer rayTracer;

    // Render the scene using the ray tracer
    rayTracer.trace(scene, film);

    // Save the rendered image to a file
    film.writeImage("triangleTest.png");
}


int main() {

//     renderBSOD();
//
//     renderSphere();
//
//     renderTriangle();
//
//     renderTransformedShapes();

//    renderComplexScene();

//    renderTriangleTest();

//  PARSED

    Parser parser = Parser();
    Scene myScene = parser.parseFile("hw3-submissionscenes/scene4-ambient.test");
    std::cout << "Scene parsed" << std::endl;
    int width = myScene.width;
    int height = myScene.height;
    Film film = Film(width, height);

    std::cout << myScene << std::endl;

    RayTracer rayTracer = RayTracer();
    rayTracer.trace(myScene, film);

    film.writeImage(parser.getOutputFilename());


    return 0;
}

