//
//
//

#ifndef RAY_TRACER_MATRIX4X4_H
#define RAY_TRACER_MATRIX4X4_H


class Matrix4x4 {
public:
    float m[4][4];

    // Constructor initializes to identity matrix
    Matrix4x4() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    }

    // Matrix multiplication
    Matrix4x4 operator*(const Matrix4x4& other) const {
        Matrix4x4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                result.m[i][j] = 0;
                for (int k = 0; k < 4; k++) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }

    Vector3 operator*(const Vector3& vec) const {
        float x = m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z + m[0][3];
        float y = m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z + m[1][3];
        float z = m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z + m[2][3];
        float w = m[3][0] * vec.x + m[3][1] * vec.y + m[3][2] * vec.z + m[3][3];

        // Convert back to 3D coordinates if w is not 1 (or close to 1 due to potential floating point inaccuracies)
        if (w != 1.0f && w != 0.0f) {
            x /= w;
            y /= w;
            z /= w;
        }

        return Vector3(x, y, z);
    }

};


#endif //RAY_TRACER_MATRIX4X4_H
