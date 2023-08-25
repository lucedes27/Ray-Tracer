//
//
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>

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
    Vector3 eye(0, -4, 4);
    Vector3 lookAt(0, 0, -2); // Center of the scene
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

    std::cout << scene << std::endl;

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
    Transform  transformSphere;
    transformSphere.scale(1, 1, 1);
    redSphere->setTransform(transformSphere.getCurrentTransform());
    scene.addObject(redSphere);
    scene.addObject(greenSphere);

    // Create a triangle and add it to the scene
    Vector3 v2(-2, -1, -3);
    Vector3 v1(-3, 1, -3);
    Vector3 v0(-1, 1, -3);

    Transform transform;
    transform.scale(1, 1, 1);
    v0 = transform.getCurrentTransform() * v0;
    v1 = transform.getCurrentTransform() * v1;
    v2 = transform.getCurrentTransform() * v2;
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

void renderTriangleTest2() {
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

    // Apply transformations to the triangle vertices
    Transform transform;
    transform.translate(1, 1, 0); // Translate by (1, 1, 0)
    transform.rotate(0, 1, 0, 45); // Rotate 45 degrees around the y-axis
    transform.scale(100, 100, 100); // Scale by 1.5 in all directions

    Matrix4x4 transformationMatrix = transform.getCurrentTransform();
    std::cout << transformationMatrix << std::endl;
    std::cout << "v0 before transformation: " << v0 << std::endl;
    v0 = transformationMatrix * v0;
    std::cout << "v0 after transformation: " << v0 << std::endl;
    std::cout << "v1 before transformation: " << v1 << std::endl;
    v1 = transformationMatrix * v1;
    std::cout << "v1 after transformation: " << v1 << std::endl;
    std::cout << "v2 before transformation: " << v2 << std::endl;
    v2 = transformationMatrix * v2;
    std::cout << "v2 after transformation: " << v2 << std::endl;

    // Create a transformed triangle and add it to the scene
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
    film.writeImage("transformedTriangleTest.png");
}

void renderTriangleTest3() {
    Vector3 eye(0, -4, 4);
    Vector3 lookAt(0, -1, 0); // Center of the scene
    Vector3 up(0, 1, 1); // Up vector
    float fovy = 45; // Field of view in y direction
    int width = 640;
    int height = 480;

    // Create the Scene object
    Scene scene(eye, lookAt, up, fovy, width, height);

    // Blue triangle material
    Material material(
            Vector3(0.0, 0.0, 0.0),
            Vector3(0.0, 0.0, 0.0),
            20.0,
            Vector3(0.0, 0.0, 0.0),
            Vector3(0.7, 0.7, 1.0)
    );

    // Define triangle vertices
    Vector3 v0(-2, -1, -0.25);
    Vector3 v1(2, -1, -0.25);
    Vector3 v2(2, -1, 0.25);

    // Create a triangle and add it to the scene
    std::shared_ptr<Shape> triangle = std::make_shared<Triangle>(v0, v1, v2, material);
    scene.addObject(triangle);

    // Create a point light and add it to the scene
    Vector3 lightPosition(1, 1, 3); // Position of the light
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
    film.writeImage("triangleTest3.png");
}

void renderEllipseTest() {
    Vector3 eye(0, -4, 4);
    Vector3 lookAt(0, -1, 0); // Center of the scene
    Vector3 up(0, 1, 1); // Up vector
    float fovy = 90; // Field of view in y direction
    int width = 1000;
    int height = 1000;

    // Create the Scene object
    Scene scene(eye, lookAt, up, fovy, width, height);

    // Ellipse material
    Material material(
            Vector3(0.0, 0.0, 0.0),
            Vector3(0.0, 0.0, 0.0),
            20.0,
            Vector3(0.0, 0.0, 0.0),
            Vector3(0.7, 0.7, 1.0)
    );

    // Create a sphere and add it to the scene
    Vector3 center(0, -1, 0);
    float radius = 1;
    std::shared_ptr<Shape> sphere = std::make_shared<Sphere>(center, radius, material);
    scene.addObject(sphere);

    // Apply scaling to the sphere to create an ellipse
    Transform transform;
    transform.scale(1.0, 0.5, 1.5); // Scale factors in x, y, and z directions
    sphere->setTransform(transform.getCurrentTransform());

    // Create a point light and add it to the scene
    Vector3 lightPosition(1, 1, 3); // Position of the light
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
    film.writeImage("ellipseTest.png");
}

void testInverse() {
    Matrix4x4 matrix;
    matrix.m[0][0] = 0.332576; matrix.m[0][1] = 0.670571; matrix.m[0][2] = 0.98035; matrix.m[0][3] = 0.188812;
    matrix.m[1][0] = 0.879688; matrix.m[1][1] = 0.920575; matrix.m[1][2] = 0.892341; matrix.m[1][3] = 0.652805;
    matrix.m[2][0] = 0.373681; matrix.m[2][1] = 0.0895065; matrix.m[2][2] = 0.23581; matrix.m[2][3] = 0.853263;
    matrix.m[3][0] = 0.54513; matrix.m[3][1] = 0.301552; matrix.m[3][2] = 0.973979; matrix.m[3][3] = 0.812752;

    Matrix4x4 inverse;
    inverse.m[0][0] = -4.35516; inverse.m[0][1] = 2.57872; inverse.m[0][2] = -3.81383; inverse.m[0][3] = 2.94444;
    inverse.m[1][0] = 2.78885; inverse.m[1][1] = -0.10476; inverse.m[1][2] = 2.65852; inverse.m[1][3] = -3.35476;
    inverse.m[2][0] = 0.294571; inverse.m[2][1] = -0.620808; inverse.m[2][2] = -1.07559; inverse.m[2][3] = 1.5594;
    inverse.m[3][0] = 1.53336; inverse.m[3][1] = -0.94678; inverse.m[3][2] = 2.86059; inverse.m[3][3] = -1.36855;

    Matrix4x4 computedInverse = matrix.inverse();

    std::cout << "Computed inverse: " << computedInverse << std::endl;
    std::cout << "Expected inverse: " << inverse << std::endl;

    // Check if the computed inverse is close to the expected inverse
    const float epsilon = 1e-3f;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (std::fabs(computedInverse.m[i][j] - inverse.m[i][j]) > epsilon) {
                std::cerr << "Inverse test failed!" << std::endl;
                return;
            }
        }
    }

    std::cout << "Inverse test passed!" << std::endl;
}

void testInverse2() {
    Matrix4x4 matrix;
    matrix.m[0][0] = 1; matrix.m[0][1] = 1; matrix.m[0][2] = 1; matrix.m[0][3] = -1;
    matrix.m[1][0] = 1; matrix.m[1][1] = 1; matrix.m[1][2] = -1; matrix.m[1][3] = 1;
    matrix.m[2][0] = 1; matrix.m[2][1] = -1; matrix.m[2][2] = 1; matrix.m[2][3] = 1;
    matrix.m[3][0] = -1; matrix.m[3][1] = 1; matrix.m[3][2] = 1; matrix.m[3][3] = 1;

    Matrix4x4 expectedInverse;
    expectedInverse.m[0][0] = 0.25; expectedInverse.m[0][1] = 0.25; expectedInverse.m[0][2] = 0.25; expectedInverse.m[0][3] = -0.25;
    expectedInverse.m[1][0] = 0.25; expectedInverse.m[1][1] = 0.25; expectedInverse.m[1][2] = -0.25; expectedInverse.m[1][3] = 0.25;
    expectedInverse.m[2][0] = 0.25; expectedInverse.m[2][1] = -0.25; expectedInverse.m[2][2] = 0.25; expectedInverse.m[2][3] = 0.25;
    expectedInverse.m[3][0] = -0.25; expectedInverse.m[3][1] = 0.25; expectedInverse.m[3][2] = 0.25; expectedInverse.m[3][3] = 0.25;
    Matrix4x4 computedInverse = matrix.inverse();

    // Check if the computed expectedInverse is close to the expected expectedInverse
    const float epsilon = 1e-5f;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (std::fabs(computedInverse.m[i][j] - expectedInverse.m[i][j]) > epsilon) {
                std::cerr << "Inverse test 2 failed!" << std::endl;
                return;
            }
        }
    }

    std::cout << "Inverse test 2 passed!" << std::endl;
}

void testTranspose() {
    Matrix4x4 matrix;
    matrix.m[0][0] = 0.332576; matrix.m[0][1] = 0.670571; matrix.m[0][2] = 0.98035; matrix.m[0][3] = 0.188812;
    matrix.m[1][0] = 0.879688; matrix.m[1][1] = 0.920575; matrix.m[1][2] = 0.892341; matrix.m[1][3] = 0.652805;
    matrix.m[2][0] = 0.373681; matrix.m[2][1] = 0.0895065; matrix.m[2][2] = 0.23581; matrix.m[2][3] = 0.853263;
    matrix.m[3][0] = 0.54513; matrix.m[3][1] = 0.301552; matrix.m[3][2] = 0.973979; matrix.m[3][3] = 0.812752;

    Matrix4x4 transpose = matrix.transpose();

    // Check if the transpose is correct
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (matrix.m[i][j] != transpose.m[j][i]) {
                std::cerr << "Transpose test failed!" << std::endl;
                return;
            }
        }
    }

    std::cout << "Transpose test passed!" << std::endl;
}

void renderSphereBehindTriangle() {
    Vector3 eye(0, -4, 4);
    Vector3 lookAt(0, 0, -2);
    Vector3 up(0, 1, 0);
    float fovy = 90;
    int width = 1000;
    int height = 1000;

    Scene scene(eye, lookAt, up, fovy, width, height);

    Material redMaterial(
            Vector3(1.0, 0.0, 0.0),
            Vector3(1.0, 1.0, 1.0),
            100.0,
            Vector3(0.0, 0.0, 0.0)
    );

    Material greenMaterial(
            Vector3(0.0, 1.0, 0.0),
            Vector3(1.0, 1.0, 1.0),
            100.0,
            Vector3(0.0, 0.0, 0.0)
    );

    // Create a sphere and add it to the scene
    std::shared_ptr<Shape> sphere = std::make_shared<Sphere>(Vector3(0, 0, -4), 1, redMaterial);
    scene.addObject(sphere);

    // Create a triangle and add it to the scene
    std::shared_ptr<Shape> triangle = std::make_shared<Triangle>(
            Vector3(-1, -1, -2),
            Vector3(1, -1, -2),
            Vector3(0, 1, -2),
            greenMaterial
    );
    scene.addObject(triangle);

    // Create a point light and add it to the scene
    Vector3 lightPosition(1, 1, 0);
    Vector3 lightColor(1.0, 1.0, 1.0);
    std::shared_ptr<Light> pointLight = std::make_shared<Light>(Light::Type::Point, lightPosition, lightColor);
    scene.addLight(pointLight);

    std::cout << scene << std::endl;

    Film film(width, height);
    RayTracer rayTracer;
    rayTracer.trace(scene, film);
    film.writeImage("sphere_behind_triangle.png");
}

void renderSpheresBehindEachOtherWithTriangle() {
    Vector3 eye(0, -4, 4);
    Vector3 lookAt(0, 0, -2);
    Vector3 up(0, 1, 0);
    float fovy = 90;
    int width = 1000;
    int height = 1000;

    Scene scene(eye, lookAt, up, fovy, width, height);

    Material redMaterial(
            Vector3(1.0, 0.0, 0.0),
            Vector3(1.0, 1.0, 1.0),
            100.0,
            Vector3(0.0, 0.0, 0.0)
    );

    Material blueMaterial(
            Vector3(0.0, 0.0, 1.0),
            Vector3(1.0, 1.0, 1.0),
            100.0,
            Vector3(0.0, 0.0, 0.0)
    );

    // Create two spheres and add them to the scene
    std::shared_ptr<Shape> sphere1 = std::make_shared<Sphere>(Vector3(0, 0, -2), 1, redMaterial);
    std::shared_ptr<Shape> sphere2 = std::make_shared<Sphere>(Vector3(0.5, 0, -3), 1, blueMaterial);
    scene.addObject(sphere1);
    scene.addObject(sphere2);

    // Create a triangle and add it to the scene
    std::shared_ptr<Shape> triangle = std::make_shared<Triangle>(
            Vector3(-2, -2, -4),
            Vector3(2, -2, -4),
            Vector3(0, 2, -4),
            redMaterial
    );
    scene.addObject(triangle);

    // Create a point light and add it to the scene
    Vector3 lightPosition(1, 1, 0);
    Vector3 lightColor(1.0, 1.0, 1.0);
    std::shared_ptr<Light> pointLight = std::make_shared<Light>(Light::Type::Point, lightPosition, lightColor);
    scene.addLight(pointLight);

    std::cout << scene << std::endl;

    Film film(width, height);
    RayTracer rayTracer;
    rayTracer.trace(scene, film);
    film.writeImage("spheres_behind_triangle.png");
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

//    renderTriangleTest2();

//    renderTriangleTest3();

//    renderEllipseTest();

//    testInverse();

//    testInverse2();

//    testTranspose();

//    renderSphereBehindTriangle();

//    renderSpheresBehindEachOtherWithTriangle();

//  PARSED

    Parser parser = Parser();
    Scene myScene = parser.parseFile("hw3-submissionscenes/scene1.test");
    int width = myScene.width;
    int height = myScene.height;
    Film film = Film(width, height);

    std::cout << myScene << std::endl;

    RayTracer rayTracer = RayTracer();
    rayTracer.trace(myScene, film);

    film.writeImage(parser.getOutputFilename());


    return 0;
}

