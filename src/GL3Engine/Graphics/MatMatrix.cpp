#include <stdarg.h>

#include "Matrix.hpp"

namespace GL3Engine {
    const Mat4& MatMatrix::translate(const Vec3& v) {
        // 1, 0, 0, v.X,
        // 0, 1, 0, v.Y,
        // 0, 0, 1, v.Z,
        // 0, 0, 0, 1
        static Mat4 t = identity();
        {
            t[3] = v.X();
            t[7] = v.Y();
            t[11] = v.Z();
        }
        return t;
    }
    const Mat4& MatMatrix::scale(const Vec3& scale) {
        // scale.X, 0, 0, 0,
        // 0, scale.Y, 0, 0,
        // 0, 0, scale.Z, 0,
        // 0, 0, 0, 1
        static Mat4 t = identity();
        {
            t[0] = scale.X();
            t[5] = scale.Y();
            t[10] = scale.Z();
        }
        return t;
    }
    const Mat4& MatMatrix::rotate(GLfloat theta, const Vec3& axis) {
        // 1 0 0 0
        // 0 1 0 0
        // 0 0 1 0
        // 0 0 0 1
        static Mat4 t = identity();
        
        GLfloat c = cosf(theta),
                s = sinf(theta),
                m_c = 1.0 - c;
        t[0] = powf(axis.X(), 2) * m_c + c;
        t[1] = axis.X() * axis.Y() * m_c - axis.Z() * s;
        t[2] = axis.X() * axis.Z() * m_c + axis.Y() * s;
        
        t[4] = axis.X() * axis.Y() * m_c + axis.Z() * s;
        t[5] = powf(axis.Y(), 2) * m_c + c;
        t[6] = axis.Y() * axis.Z() * m_c - axis.X() * s;
        
        t[8] = axis.X() * axis.Z() * m_c - axis.Y() * s;
        t[9] = axis.Y() * axis.Z() * m_c + axis.X() * s;
        t[10] = powf(axis.Z(), 2) * m_c + c;
        
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
                        c / aspect, 0, 0, 0,
                        0, c, 0, 0,
                        0, 0, (far + near) / (near - far), (2 * far * near)
                                / (near - far),
                        0, 0, -1, 0 });
    }
    Mat4 MatMatrix::lookAt(
            const Vec4& _eye, const Vec4& _target, const Vec4& _dir) {
        Vec4 eye = -_eye;
        Vec4 z_c = normalize(_eye - _target);
        Vec4 y_c = normalize(_dir);
        Vec4 x_c = normalize(cross(y_c, z_c));

        y_c = cross(z_c, x_c);
        eye[3] = 1.f;

        return Mat4(
                {
                        x_c[0], x_c[1], x_c[2], dot(x_c, eye),
                        y_c[0], y_c[1], y_c[2], dot(y_c, eye),
                        z_c[0], z_c[1], z_c[2], dot(z_c, eye),
                        0, 0, 0, 1 }
                );
    }
    Mat4 MatMatrix::orthof(const array<Vec2, 3>& array) {
        GLfloat _x = array[0].Y() - array[0].X(),
                _y = array[1].Y() - array[1].X(),
                _z = array[2].Y() - array[2].X();
        return Mat4(
                {
                        2.f / _x, 0, 0, -(array[0].Y() + array[0].X()) / _x,
                        0, 2.f / _y, 0, -(array[1].Y() + array[1].X()) / _y,
                        0, 0, -2.f / _z, -(array[2].Y() + array[2].X()) / _z,
                        0, 0, 0, 1.f });
    }
    
    /** Obliczenia dla normal matrix */
    void MatMatrix::transpose(const fMat& matrix, GLfloat* array) {
        for (GLuint i = 0; i < matrix.cols; ++i)
            for (GLuint j = 0; j < matrix.rows; ++j)
                array[i * matrix.rows + j] = matrix.matrix[j * matrix.cols + i];
    }
    fMat MatMatrix::transpose(const fMat& matrix) {
        fMat result(matrix.rows, matrix.cols);
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
            memcpy(va_arg(vl, fMat*) ->matrix, mat, 16 * sizeof(GLfloat));
        va_end(vl);
    }
    
    void MatMatrix::translate(fMat& matrix, const Vec3& v) {
        matrix *= translate(v);
    }
    void MatMatrix::scale(fMat& matrix, const Vec3& _scale) {
        matrix *= scale(_scale);
    }
    void MatMatrix::rotate(fMat& matrix, GLfloat theta, const Vec3& axis) {
        matrix *= rotate(theta, axis);
    }

    /** Dla quaternionów */
    void MatMatrix::calcW(Vec4& v) {
        GLfloat t = 1.0f - length(v);
        v[3] = t < 0.f ? 0.f : -sqrtf(t);
    }
    Vec4 MatMatrix::calcW(const Vec4& v) {
        Vec4 _v = v;
        calcW(_v);
        return _v;
    }

    void MatMatrix::normalize(Vec4& v) {
        GLfloat l = length(v);
        for (GLuint i = 0; i < 3; ++i)
            v[i] /= l;
    }
    Vec4 MatMatrix::normalize(const Vec4& v) {
        Vec4 _v = v;
        normalize(_v);
        return _v;
    }

    Vec4 MatMatrix::cross(const Vec3& a, const Vec3& b) {
        return {
            a.Y() * b.Z() - a.Z() * b.Y(),
            a.Z() * b.X() - a.X() * b.Z(),
            a.X() * b.Y() - a.Y() * b.X()
        };
    }
    GLfloat MatMatrix::dot(
            const Vec3& a, const Vec3& b) {
        return a.X() * b.X() + a.Y() * b.Y() + a.Z() * b.Z();
    }
    GLfloat MatMatrix::distance(const Vec3& a, const Vec3& b) {
        return sqrt((a.X() - b.X()) * (a.X() - b.X()) +
                (a.Y() - b.Y()) * (a.Y() - b.Y()) +
                (a.Z() - b.Z()) * (a.Z() - b.Z()));
    }
}

