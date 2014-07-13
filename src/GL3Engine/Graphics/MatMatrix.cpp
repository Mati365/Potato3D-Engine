#include "Matrix.hpp"

namespace GL3Engine {
    template<typename T>
    Mat4 MatMatrix<T>::translate(const FPoint3D& v) {
        return Mat4( {
                1, 0, 0, v.X,
                0, 1, 0, v.Y,
                0, 0, 1, v.Z,
                0, 0, 0, 1
        });
    }
    template<typename T>
    Mat4 MatMatrix<T>::scale(const FPoint3D& scale) {
        return Mat4( {
                scale.X, 0, 0, 0,
                0, scale.Y, 0, 0,
                0, 0, scale.Z, 0,
                0, 0, 0, 1
        });
    }
    template<typename T>
    Mat4 MatMatrix<T>::rotate(GLfloat theta,
            const FPoint3D& axis) {
        float c = cosf(theta), s = sinf(theta), m_c = 1.0 - c;
        return Mat4( {
                // Wers 1
                (float) pow(axis.X, 2) * m_c + c,
                axis.X * axis.Y * m_c - axis.Z * s,
                axis.X * axis.Z * m_c + axis.Y * s,
                0,
                // Wers 2
                axis.X * axis.Y * m_c + axis.Z * s,
                (float) pow(axis.Y, 2) * m_c + c,
                axis.Y * axis.Z * m_c - axis.X * s,
                0,
                // Wers 3
                axis.X * axis.Z * m_c - axis.Y * s,
                axis.Y * axis.Z * m_c + axis.X * s,
                (float) pow(axis.Z, 2) * m_c + c,
                0,
                // Wers 4
                0, 0, 0, 1
        });
    }

    /**
     * Obliczenia dla MVP
     * Wzory macierzy:
     * http://www.3dcpptutorials.sk/index.php?id=2
     */
    template<typename T>
    Mat4 MatMatrix<T>::perspective(GLfloat angle, GLfloat aspect, GLfloat near,
            GLfloat far) {
        GLfloat c = cotan(angle / 2.0);
        return Mat4(
                {
                        c / aspect, 0, 0, 0,
                        0, c, 0, 0,
                        0, 0, (far + near) / (near - far), (2 * far
                                * near) / (near - far),
                        0, 0, -1, 0
                });
    }
    template<typename T>
    Mat4 MatMatrix<T>::lookAt(const FPoint3D& _eye,
            const FPoint3D& _target,
            const FPoint3D& _dir) {
        FPoint3D z_c = normalize(_eye - _target);
        FPoint3D y_c = normalize(_dir);
        FPoint3D x_c = normalize(cross(y_c, z_c));
        y_c = cross(z_c, x_c);

        return Mat4( {
                x_c.X, x_c.Y, x_c.Z, -dot(x_c, _eye),
                y_c.X, y_c.Y, y_c.Z, -dot(y_c, _eye),
                z_c.X, z_c.Y, z_c.Z, -dot(z_c, _eye),
                0, 0, 0, 1
        });
    }
    template<typename T>
    Mat4 MatMatrix<T>::orthof(const array<FPoint2D, 3>& array) {
        GLfloat _x = array[0].Y - array[0].X,
                _y = array[1].Y - array[1].X,
                _z = array[2].Y - array[2].X;
        return Mat4( {
                2.f / _x, 0, 0, -(array[0].Y + array[0].X) / _x,
                0, 2.f / _y, 0, -(array[1].Y + array[1].X) / _y,
                0, 0, -2.f / _z, -(array[2].Y + array[2].X) / _z,
                0, 0, 0, 1.f
        });
    }

    /** Obliczenia dla normal matrix */
    template<typename T>
    void MatMatrix<T>::transpose(const Matrix<T>& matrix, T* array) {
        for (GLint i = 0; i < matrix.cols; ++i)
            for (GLint j = 0; j < matrix.rows; ++j)
                array[i * matrix.rows + j] = matrix.matrix[j
                        * matrix.cols + i];
    }
    template<typename T>
    Matrix<T> MatMatrix<T>::transpose(const Matrix<T>& matrix) {
        Matrix<T> result(matrix.rows, matrix.cols);
        transpose(matrix, result.matrix);
        return result;
    }

    template<typename T>
    void MatMatrix<T>::inverse(Mat3* matrix) {
        T* arr = matrix->matrix;
        GLfloat det = (arr[4] * arr[8] - arr[5] * arr[7]) -
                2 * (arr[3] * arr[8] - arr[5] * arr[6]) +
                3 * (arr[3] * arr[7] - arr[4] * arr[6]);
        if (det == 0)
            return;

        // Rotacja
        static T mat[9];
        transpose(*matrix, mat);

        arr[0] = mat[4] * mat[8] - mat[5] * mat[7];
        arr[1] = -mat[3] * mat[8] + mat[5] * mat[6];
        arr[2] = mat[3] * mat[7] - mat[4] * mat[6];
        arr[3] = -mat[1] * mat[8] + mat[2] * mat[7];
        arr[4] = mat[0] * mat[8] - mat[2] * mat[6];
        arr[5] = -mat[0] * mat[7] + mat[1] * mat[6];
        arr[6] = mat[1] * mat[5] - mat[2] * mat[4];
        arr[7] = -mat[0] * mat[5] + mat[2] * mat[3];
        arr[8] = mat[0] * mat[4] - mat[1] * mat[3];
    }
    template<typename T>
    Mat3 MatMatrix<T>::inverse(const Mat3& matrix) {
        Mat3 mat(matrix);
        inverse(&mat);
        return mat;
    }

    /** INLINE */
    template<typename T>
    void MatMatrix<T>::identity(Matrix<T>& matrix) {
        matrix = identity();
    }
    template<typename T>
    void MatMatrix<T>::translate(Matrix<T>& matrix,
            const FPoint3D& v) {
        matrix *= translate(v);
    }
    template<typename T>
    void MatMatrix<T>::scale(Matrix<T>& matrix,
            const FPoint3D& _scale) {
        matrix *= scale(_scale);
    }
    template<typename T>
    void MatMatrix<T>::rotate(Matrix<T>& matrix, GLfloat theta,
            const FPoint3D& axis) {
        matrix *= rotate(theta, axis);
    }

    template class MatMatrix<GLfloat> ;
}

