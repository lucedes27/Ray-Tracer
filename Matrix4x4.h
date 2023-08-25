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

    void getCofactor(float temp[4][4], int p, int q, int n) const {
        int i = 0, j = 0;
        for (int row = 0; row < n; row++) {
            for (int col = 0; col < n; col++) {
                if (row != p && col != q) {
                    temp[i][j++] = m[row][col];
                    if (j == n - 1) {
                        j = 0;
                        i++;
                    }
                }
            }
        }
    }

    float determinant(int n) const {
        float D = 0;
        if (n == 1)
            return m[0][0];

        float temp[4][4];
        int sign = 1;

        for (int f = 0; f < n; f++) {
            getCofactor(temp, 0, f, n);
            D += sign * m[0][f] * Matrix4x4(temp).determinant(n - 1);
            sign = -sign;
        }

        return D;
    }

    void adjoint(float adj[4][4]) const {
        if (4 == 1) {
            adj[0][0] = 1;
            return;
        }

        int sign = 1;
        float temp[4][4];

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                getCofactor(temp, i, j, 4);
                sign = ((i + j) % 2 == 0) ? 1 : -1;
                adj[j][i] = (sign) * (Matrix4x4(temp).determinant(4 - 1));
            }
        }
    }

    Matrix4x4 inverse() const {
        float det = determinant(4);
        if (det == 0) {
            std::cout << "Singular matrix, can't find its inverse";
            return *this; // Return the original matrix if it's singular
        }

        float adj[4][4];
        adjoint(adj);

        Matrix4x4 inv;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                inv.m[i][j] = adj[i][j] / det;

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


private:
    // Constructor that takes a 4x4 array
    Matrix4x4(float arr[4][4]) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                m[i][j] = arr[i][j];
            }
        }
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

std::ostream& operator<<(std::ostream& os, const Matrix4x4& matrix) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            os << matrix.m[i][j];
            if (j != 3) os << ", ";
        }
        os << std::endl;
    }
    return os;
}

#endif //RAY_TRACER_MATRIX4X4_H
