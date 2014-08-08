#include <stdarg.h>

#include "Matrix.hpp"

namespace GL3Engine {
    const Mat4& MatMatrix::translate(const FPoint3D& v) {
        // 1, 0, 0, v.X,
        // 0, 1, 0, v.Y,
        // 0, 0, 1, v.Z,
        // 0, 0, 0, 1
        static Mat4 t = identity();
        {
            t[3] = v.X;
            t[7] = v.Y;
            t[11] = v.Z;
        }
        return t;
    }
    const Mat4& MatMatrix::scale(const FPoint3D& scale) {
        // scale.X, 0, 0, 0,
        // 0, scale.Y, 0, 0,
        // 0, 0, scale.Z, 0,
        // 0, 0, 0, 1
        static Mat4 t = identity();
        {
            t[0] = scale.X;
            t[5] = scale.Y;
            t[10] = scale.Z;
        }
        return t;
    }
    const Mat4& MatMatrix::rotate(GLfloat theta, const FPoint3D& axis) {
        // 1 0 0 0
        // 0 1 0 0
        // 0 0 1 0
        // 0 0 0 1
        static Mat4 t = identity();
        
        GLfloat c = cosf(theta), s = sinf(theta), m_c = 1.0 - c;
        t[0] = (GLfloat) pow(axis.X, 2) * m_c + c;
        t[1] = axis.X * axis.Y * m_c - axis.Z * s;
        t[2] = axis.X * axis.Z * m_c + axis.Y * s;
        
        t[4] = axis.X * axis.Y * m_c + axis.Z * s;
        t[5] = (GLfloat) pow(axis.Y, 2) * m_c + c;
        t[6] = axis.Y * axis.Z * m_c - axis.X * s;
        
        t[8] = axis.X * axis.Z * m_c - axis.Y * s;
        t[9] = axis.Y * axis.Z * m_c + axis.X * s;
        t[10] = (GLfloat) pow(axis.Z, 2) * m_c + c;
        
        return t;
    }
    
    /**
     * Obliczenia dla MVP
     * Wzory macierzy:
     * http://www.3dcpptutorials.sk/index.php?id=2
     */
    Mat4 MatMatrix::perspective(
            GLfloat angle, GLfloat aspect, GLfloat near, GLfloat far) {
        GLfloat c = cotan(angle / 2.0);
        return Mat4(
                {
                        c / aspect, 0, 0, 0, 0, c, 0, 0, 0, 0, (far + near)
                                / (near - far), (2 * far * near) / (near - far),
                        0, 0, -1, 0 });
    }
    Mat4 MatMatrix::lookAt(
            const FPoint3D& _eye, const FPoint3D& _target,
            const FPoint3D& _dir) {
        FPoint3D z_c = normalize(_eye - _target);
        FPoint3D y_c = normalize(_dir);
        FPoint3D x_c = normalize(cross(y_c, z_c));
        y_c = cross(z_c, x_c);
        
        return Mat4(
                {
                        x_c.X, x_c.Y, x_c.Z, dot(x_c, -_eye), y_c.X, y_c.Y, y_c
                                .Z, dot(y_c, -_eye), z_c.X, z_c.Y, z_c.Z, dot(
                                z_c, -_eye), 0, 0, 0, 1 });
    }
    Mat4 MatMatrix::orthof(const array<FPoint2D, 3>& array) {
        GLfloat _x = array[0].Y - array[0].X, _y = array[1].Y - array[1].X, _z =
                array[2].Y - array[2].X;
        return Mat4(
                {
                        2.f / _x, 0, 0, -(array[0].Y + array[0].X) / _x, 0, 2.f
                                / _y, 0, -(array[1].Y + array[1].X) / _y, 0, 0,
                        -2.f / _z, -(array[2].Y + array[2].X) / _z, 0, 0, 0, 1.f });
    }
    
    /** Obliczenia dla normal matrix */
    void MatMatrix::transpose(const FMat& matrix, GLfloat* array) {
        for (GLuint i = 0; i < matrix.cols; ++i)
            for (GLuint j = 0; j < matrix.rows; ++j)
                array[i * matrix.rows + j] = matrix.matrix[j * matrix.cols + i];
    }
    FMat MatMatrix::transpose(const FMat& matrix) {
        FMat result(matrix.rows, matrix.cols);
        transpose(matrix, result.matrix);
        return result;
    }
    
    void MatMatrix::inverse(Mat3* matrix) {
        GLfloat* arr = matrix->matrix;
        GLfloat det = (arr[4] * arr[8] - arr[5] * arr[7])
                - 2 * (arr[3] * arr[8] - arr[5] * arr[6])
                + 3 * (arr[3] * arr[7] - arr[4] * arr[6]);
        if (det == 0)
            return;
        
        // Rotacja
        static GLfloat mat[9];
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
    Mat3 MatMatrix::inverse(const Mat3& matrix) {
        Mat3 mat(matrix);
        inverse(&mat);
        return mat;
    }
    
    /** Podstawowe */
    void MatMatrix::identity(GLuint count, ...) {
        static const GLfloat mat[16] = {
                1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
        va_list vl;
        va_start(vl, count);
        for (GLuint i = 0; i < count; ++i)
            memcpy(va_arg(vl, FMat*) ->matrix, mat, 16 * sizeof(GLfloat));
        va_end(vl);
    }
    
    void MatMatrix::translate(FMat& matrix, const FPoint3D& v) {
        matrix *= translate(v);
    }
    void MatMatrix::scale(FMat& matrix, const FPoint3D& _scale) {
        matrix *= scale(_scale);
    }
    void MatMatrix::rotate(FMat& matrix, GLfloat theta, const FPoint3D& axis) {
        matrix *= rotate(theta, axis);
    }
}

