//
//
//

#ifndef RAY_TRACER_PARSER_H
#define RAY_TRACER_PARSER_H

#include "Scene.h"
#include "Sphere.h"
#include "Triangle.h"
#include "Material.h"
#include "Transform.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stack>

class Parser {
private:
    Scene scene;
    int width, height;
    std::string outputFilename;
    float lookfromx, lookfromy, lookfromz, lookatx, lookaty, lookatz, upx, upy, upz, fov;
    float constantAttenuation, linearAttenuation, quadraticAttenuation;

    Material material;

    Transform transform;
    std::stack<Transform> transformStack;  // Stack to store transformations

    Vector3* vertices;
    int vertexCount;
public:
    Parser() : width(0), height(0), outputFilename(""), lookfromx(0), lookfromy(0), lookfromz(0), lookatx(0), lookaty(0),
               lookatz(0), upx(0), upy(0), upz(0), fov(0), constantAttenuation(1), linearAttenuation(0),
               quadraticAttenuation(0), vertexCount(0) {}


    ~Parser() {
        delete[] vertices;
    }

    Scene parseFile(const std::string& filename) {
        std::cout << "Parsing file " << filename << std::endl;
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file");
        }
        std::string line;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string command;
            iss >> command;
            if (command == "#") {
                // Comment. Skip the line.
                continue;
            } else if (command == "size") {
                iss >> width >> height;
                scene.width = width;
                scene.height = height;
            } else if (command == "maxdepth") {
                int maxDepth;
                iss >> maxDepth;
                scene.setMaxRecursionDepth(maxDepth);
            } else if (command == "output") {
                iss >> outputFilename;
            } else if (command == "camera") {
                iss >> lookfromx >> lookfromy >> lookfromz >> lookatx >> lookaty >> lookatz >> upx >> upy >> upz >> fov;
                scene.setEyePosition(Vector3(lookfromx, lookfromy, lookfromz));
                scene.setLookAt(Vector3(lookatx, lookaty, lookatz));
                scene.setUp(Vector3(upx, upy, upz));
                scene.setFov(fov);
            } else if (command == "sphere") {
                float x, y, z, radius;
                iss >> x >> y >> z >> radius;
                Vector3 transformedCenter = transform.getCurrentTransform() * Vector3(x, y, z);
                scene.addObject(std::make_shared<Sphere>(transformedCenter, radius, material));
            } else if (command == "maxverts") {
                int maxverts;
                iss >> maxverts;
                vertices = new Vector3[maxverts];
            } else if (command == "vertex") {
                float x, y, z;
                iss >> x >> y >> z;
                // Store vertex in vertices
                vertices[vertexCount] = Vector3(x, y, z);
                vertexCount++;
            } else if (command == "tri") {
                int v1, v2, v3;
                iss >> v1 >> v2 >> v3;
                Vector3 transformedV1 = transform.getCurrentTransform() * vertices[v1];
                Vector3 transformedV2 = transform.getCurrentTransform() * vertices[v2];
                Vector3 transformedV3 = transform.getCurrentTransform() * vertices[v3];
                scene.addObject(std::make_shared<Triangle>(transformedV1, transformedV2, transformedV3, material));
            } else if (command == "translate") {
                float x, y, z;
                iss >> x >> y >> z;
                transform.translate(x, y, z);
            } else if (command == "rotate") {
                float x, y, z, angle;
                iss >> x >> y >> z >> angle;
                transform.rotate(x, y, z, angle);
            } else if (command == "scale") {
                float x, y, z;
                iss >> x >> y >> z;
                transform.scale(x, y, z);
            } else if (command == "directional") {
                float x, y, z, r, g, b;
                iss >> x >> y >> z >> r >> g >> b;
                scene.addLight(std::make_shared<Light>(Light::Type::Directional, Vector3(x, y, z), Vector3(r, g, b)));
            } else if (command == "point") {
                float x, y, z, r, g, b;
                iss >> x >> y >> z >> r >> g >> b;
                scene.addLight(std::make_shared<Light>(Light::Type::Point, Vector3(x, y, z), Vector3(r, g, b)));
            } else if (command == "attenuation") {
                iss >> constantAttenuation >> linearAttenuation >> quadraticAttenuation;
                scene.setAttenuation(constantAttenuation, linearAttenuation, quadraticAttenuation);
            } else if (command == "ambient") {
                float r, g, b;
                iss >> r >> g >> b;
                material.setAmbient(Vector3(r, g, b));
            } else if (command == "diffuse") {
                float r, g, b;
                iss >> r >> g >> b;
                material.setDiffuse(Vector3(r, g, b));
            } else if (command == "specular") {
                float r, g, b;
                iss >> r >> g >> b;
                material.setSpecular(Vector3(r, g, b));
            } else if (command == "shininess") {
                float s;
                iss >> s;
                material.setShininess(s);
            } else if (command == "emission") {
                float r, g, b;
                iss >> r >> g >> b;
                material.setEmission(Vector3(r, g, b));
            } else if (command == "pushTransform") {
                transform.pushTransform();
            } else if (command == "popTransform") {
                transform.popTransform();
            }
        }

        std::cout << "Done parsing" << std::endl;
        scene.setFovX();
        scene.updateVirtualScreen();
        return scene;
    }

    std::string getOutputFilename() const {
        return outputFilename;
    }
};


#endif //RAY_TRACER_PARSER_H
