//
//
//

#ifndef RAY_TRACER_TRANSFORM_H
#define RAY_TRACER_TRANSFORM_H

#include "Matrix4x4.h"
#include <stack>
#include <cmath>

class Transform {
private:
    std::stack<Matrix4x4> transformStack;
    Matrix4x4 currentTransform;

public:
    Transform() {
        // Initialize with identity matrix
        transformStack.push(Matrix4x4());
    }

    void translate(float x, float y, float z) {
        Matrix4x4 translation;
        translation.m[0][3] = x;
        translation.m[1][3] = y;
        translation.m[2][3] = z;
        currentTransform = currentTransform * translation;
    }

    void rotate(float x, float y, float z, float angle) {
        // Convert angle to radians
        float rad = angle * M_PI / 180.0f;
        float c = cos(rad);
        float s = sin(rad);
        float omc = 1.0f - c;

        Matrix4x4 rotation;

        // Normalize the axis
        float magnitude = sqrt(x*x + y*y + z*z);
        x /= magnitude;
        y /= magnitude;
        z /= magnitude;

        // Implement rotation matrix based on axis and angle
        rotation.m[0][0] = x*x*omc + c;
        rotation.m[0][1] = x*y*omc - z*s;
        rotation.m[0][2] = x*z*omc + y*s;
        rotation.m[0][3] = 0;

        rotation.m[1][0] = x*y*omc + z*s;
        rotation.m[1][1] = y*y*omc + c;
        rotation.m[1][2] = y*z*omc - x*s;
        rotation.m[1][3] = 0;

        rotation.m[2][0] = x*z*omc - y*s;
        rotation.m[2][1] = y*z*omc + x*s;
        rotation.m[2][2] = z*z*omc + c;
        rotation.m[2][3] = 0;

        rotation.m[3][0] = 0;
        rotation.m[3][1] = 0;
        rotation.m[3][2] = 0;
        rotation.m[3][3] = 1;

        currentTransform = currentTransform * rotation;
    }

    void scale(float x, float y, float z) {
        Matrix4x4 scaling;
        scaling.m[0][0] = x;
        scaling.m[1][1] = y;
        scaling.m[2][2] = z;
        currentTransform = currentTransform * scaling;
    }

    void pushTransform() {
        transformStack.push(currentTransform);
        currentTransform = Matrix4x4();  // Reset currentTransform to the identity matrix
    }

    void popTransform() {
        if (!transformStack.empty()) {
            currentTransform = transformStack.top();
            transformStack.pop();
        } else {
            // Handle error: Trying to pop from an empty stack
            std::cerr << "Error: Transformation stack is empty. Cannot pop." << std::endl;
        }
    }

    Matrix4x4 getCurrentTransform() const {
        return currentTransform;
    }

    Matrix4x4 getInverseTransform() const {
        return currentTransform.inverse();
    }
};


#endif //RAY_TRACER_TRANSFORM_H
