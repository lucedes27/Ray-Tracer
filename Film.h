//
//
//

#ifndef RAY_TRACER_FILM_H
#define RAY_TRACER_FILM_H

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

class Film {
public:
    int width, height;
    std::vector<Vector3> pixels;

    Film(int width, int height) : width(width), height(height), pixels(width * height, Vector3(0, 0, 0)) {}

    void addSample(int x, int y, const Vector3& color) {
        pixels[y * width + x] = color;
    }

    void writeImage(const std::string& filename) const {
        std::vector<uint8_t> image(width * height * 3);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                Vector3 color = pixels[y * width + x];
                image[(y * width + x) * 3 + 0] = static_cast<uint8_t>(color.x * 255);
                image[(y * width + x) * 3 + 1] = static_cast<uint8_t>(color.y * 255);
                image[(y * width + x) * 3 + 2] = static_cast<uint8_t>(color.z * 255);
            }
        }
        stbi_write_png(filename.c_str(), width, height, 3, image.data(), width * 3);
    }
};



#endif //RAY_TRACER_FILM_H
