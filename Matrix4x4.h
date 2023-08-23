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

        const float epsilon = 1e-5f;  // Some small value
        if (fabs(w - 1.0f) > epsilon && fabs(w) > epsilon) {
            x /= w;
            y /= w;
            z /= w;
        }

        return Vector3(x, y, z);
    }

    Matrix4x4 inverse() const {
        Matrix4x4 inv;
        float det;

        inv.m[0][0] = m[1][1]  * m[2][2] * m[3][3] -
                      m[1][1]  * m[2][3] * m[3][2] -
                      m[2][1]  * m[1][2]  * m[3][3] +
                      m[2][1]  * m[1][3]  * m[3][2] +
                      m[3][1] * m[1][2]  * m[2][3] -
                      m[3][1] * m[1][3]  * m[2][2];

        inv.m[1][0] = -m[1][0]  * m[2][2] * m[3][3] +
                      m[1][0]  * m[2][3] * m[3][2] +
                      m[2][0]  * m[1][2]  * m[3][3] -
                      m[2][0]  * m[1][3]  * m[3][2] -
                      m[3][0] * m[1][2]  * m[2][3] +
                      m[3][0] * m[1][3]  * m[2][2];

        inv.m[2][0] = m[1][0]  * m[2][1] * m[3][3] -
                      m[1][0]  * m[2][3] * m[3][1] -
                      m[2][0]  * m[1][1] * m[3][3] +
                      m[2][0]  * m[1][3] * m[3][1] +
                      m[3][0] * m[1][1] * m[2][3] -
                      m[3][0] * m[1][3] * m[2][1];

        inv.m[3][0] = -m[1][0]  * m[2][1] * m[3][2] +
                      m[1][0]  * m[2][2] * m[3][1] +
                      m[2][0]  * m[1][1] * m[3][2] -
                      m[2][0]  * m[1][2] * m[3][1] -
                      m[3][0] * m[1][1] * m[2][2] +
                      m[3][0] * m[1][2] * m[2][1];

        inv.m[0][1] = -m[0][1]  * m[2][2] * m[3][3] +
                      m[0][1]  * m[2][3] * m[3][2] +
                      m[2][1]  * m[0][2] * m[3][3] -
                      m[2][1]  * m[0][3] * m[3][2] -
                      m[3][1] * m[0][2] * m[2][3] +
                      m[3][1] * m[0][3] * m[2][2];

        inv.m[1][1] = m[0][0]  * m[2][2] * m[3][3] -
                      m[0][0]  * m[2][3] * m[3][2] -
                      m[2][0]  * m[0][2] * m[3][3] +
                      m[2][0]  * m[0][3] * m[3][2] +
                      m[3][0] * m[0][2] * m[2][3] -
                      m[3][0] * m[0][3] * m[2][2];

        inv.m[2][1] = -m[0][0]  * m[2][1] * m[3][3] +
                      m[0][0]  * m[2][3] * m[3][1] +
                      m[2][0]  * m[0][1] * m[3][3] -
                      m[2][0]  * m[0][3] * m[3][1] -
                      m[3][0] * m[0][1] * m[2][3] +
                      m[3][0] * m[0][3] * m[2][1];

        inv.m[3][1] = m[0][0]  * m[2][1] * m[3][2] -
                      m[0][0]  * m[2][2] * m[3][1] -
                      m[2][0]  * m[0][1] * m[3][2] +
                      m[2][0]  * m[0][2] * m[3][1] +
                      m[3][0] * m[0][1] * m[2][2] -
                      m[3][0] * m[0][2] * m[2][1];

        inv.m[0][2] = m[0][1]  * m[1][2] * m[3][3] -
                      m[0][1]  * m[1][3] * m[3][2] -
                      m[1][1]  * m[0][2] * m[3][3] +
                      m[1][1]  * m[0][3] * m[3][2] +
                      m[3][1] * m[0][2] * m[1][3] -
                      m[3][1] * m[0][3] * m[1][2];

        inv.m[1][2] = -m[0][0]  * m[1][2] * m[3][3] +
                      m[0][0]  * m[1][3] * m[3][2] +
                      m[1][0]  * m[0][2] * m[3][3] -
                      m[1][0]  * m[0][3] * m[3][2] -
                      m[3][0] * m[0][2] * m[1][3] +
                      m[3][0] * m[0][3] * m[1][2];

        inv.m[2][2] = m[0][0]  * m[1][1] * m[3][3] -
                      m[0][0]  * m[1][3] * m[3][1] -
                      m[1][0]  * m[0][1] * m[3][3] +
                      m[1][0]  * m[0][3] * m[3][1] +
                      m[3][0] * m[0][1] * m[1][3] -
                      m[3][0] * m[0][3] * m[1][1];

        inv.m[3][2] = -m[0][0]  * m[1][1] * m[3][2] +
                      m[0][0]  * m[1][2] * m[3][1] +
                      m[1][0]  * m[0][1] * m[3][2] -
                      m[1][0]  * m[0][2] * m[3][1] -
                      m[3][0] * m[0][1] * m[1][2] +
                      m[3][0] * m[0][2] * m[1][1];

        inv.m[0][3] = -m[0][1] * m[1][2] * m[2][3] +
                      m[0][1] * m[1][3] * m[2][2] +
                      m[1][1] * m[0][2] * m[2][3] -
                      m[1][1] * m[0][3] * m[2][2] -
                      m[2][1] * m[0][2] * m[1][3] +
                      m[2][1] * m[0][3] * m[1][2];

        inv.m[1][3] = m[0][0] * m[1][2] * m[2][3] -
                      m[0][0] * m[1][3] * m[2][2] -
                      m[1][0] * m[0][2] * m[2][3] +
                      m[1][0] * m[0][3] * m[2][2] +
                      m[2][0] * m[0][2] * m[1][3] -
                      m[2][0] * m[0][3] * m[1][2];

        inv.m[2][3] = -m[0][0] * m[1][1] * m[2][3] +
                      m[0][0] * m[1][3] * m[2][1] +
                      m[1][0] * m[0][1] * m[2][3] -
                      m[1][0] * m[0][3] * m[2][1] -
                      m[2][0] * m[0][1] * m[1][3] +
                      m[2][0] * m[0][3] * m[1][1];

        inv.m[3][3] = m[0][0] * m[1][1] * m[2][2] -
                      m[0][0] * m[1][2] * m[2][1] -
                      m[1][0] * m[0][1] * m[2][2] +
                      m[1][0] * m[0][2] * m[2][1] +
                      m[2][0] * m[0][1] * m[1][2] -
                      m[2][0] * m[0][2] * m[1][1];

        det = m[0][0] * inv.m[0][0] + m[0][1] * inv.m[1][0] + m[0][2] * inv.m[2][0] + m[0][3] * inv.m[3][0];

        if (det == 0)
            return *this; // Return the original matrix if it's singular

        det = 1.0 / det;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                inv.m[i][j] = inv.m[i][j] * det;
            }
        }

        return inv;
    }

    Matrix4x4 transpose() const {
        Matrix4x4 transposedMatrix;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                transposedMatrix.m[i][j] = m[j][i];
            }
        }
        return transposedMatrix;
    }

};

bool operator==(const Matrix4x4& lhs, const Matrix4x4& rhs) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (lhs.m[i][j] != rhs.m[i][j]) {
                return false;
            }
        }
    }
    return true;
}

#endif //RAY_TRACER_MATRIX4X4_H
