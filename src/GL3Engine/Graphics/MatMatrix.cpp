#include <stdarg.h>

#include "Matrix.hpp"
#include "AttribContainer.hpp"

namespace GL3Engine {
    namespace CoreMatrix {
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
        Mat4 MatMatrix::orthof(const std::array<Vec2, 3>& array) {
            GLfloat _x = array[0].Y() - array[0].X(),
                    _y = array[1].Y() - array[1].X(),
                    _z = array[2].Y() - array[2].X();
            return Mat4(
                    {
                            2.f / _x, 0, 0, -(array[0].Y() + array[0].X()) / _x,
                            0, 2.f / _y, 0, -(array[1].Y() + array[1].X()) / _y,
                            0, 0, -2.f / _z, -(array[2].Y() + array[2].X())
                                    / _z,
                            0, 0, 0, 1.f });
        }
        
        /** Obliczenia dla normal matrix */
        void MatMatrix::transpose(GLfloat* array, GLuint rows, GLuint cols) {
            GLfloat* temp = new GLfloat[rows * cols];
            for (GLuint i = 0; i < cols; ++i)
                for (GLuint j = 0; j < rows; ++j)
                    temp[j * cols + i] = array[i * rows + j];
            memcpy(array, temp, rows * cols * sizeof(GLfloat));
            Tools::safeDelete(temp, true);
        }
        fMat MatMatrix::transpose(const fMat& matrix) {
            fMat result = matrix;
            transpose(result.matrix, matrix.rows, matrix.cols);
            return result;
        }
        
        void MatMatrix::inverse(Mat3* matrix) {
//        http://stackoverflow.com/questions/983999/simple-3x3-matrix-inverse-code-c
//        double determinant =    +A(0,0)*(A(1,1)*A(2,2)-A(2,1)*A(1,2))
//                                -A(0,1)*(A(1,0)*A(2,2)-A(1,2)*A(2,0))
//                                +A(0,2)*(A(1,0)*A(2,1)-A(1,1)*A(2,0));
//        0x0=0 1x0=1 2x0=2
//        0x1=3 1x1=4 2x1=5
//        0x2=6 1x2=7 2x2=8
            GLfloat* arr = matrix->matrix;

            static GLfloat mat[9];
            memcpy(mat, matrix->matrix, 9 * sizeof(GLfloat));

            GLfloat det =
                    arr[0] * (arr[4] * arr[8] - arr[5] * arr[7])
                            - arr[3] * (arr[1] * arr[8] - arr[7] * arr[2])
                            + arr[6] * (arr[1] * arr[5] - arr[4] * arr[2]);
            GLfloat inv_det = 1.f / det;

//        double invdet = 1/determinant;
//        result(0,0) =  (A(1,1)*A(2,2)-A(2,1)*A(1,2))*invdet;
//        result(1,0) = -(A(0,1)*A(2,2)-A(0,2)*A(2,1))*invdet;
//        result(2,0) =  (A(0,1)*A(1,2)-A(0,2)*A(1,1))*invdet;
//        result(0,1) = -(A(1,0)*A(2,2)-A(1,2)*A(2,0))*invdet;
//        result(1,1) =  (A(0,0)*A(2,2)-A(0,2)*A(2,0))*invdet;
//        result(2,1) = -(A(0,0)*A(1,2)-A(1,0)*A(0,2))*invdet;
//        result(0,2) =  (A(1,0)*A(2,1)-A(2,0)*A(1,1))*invdet;
//        result(1,2) = -(A(0,0)*A(2,1)-A(2,0)*A(0,1))*invdet;
//        result(2,2) =  (A(0,0)*A(1,1)-A(1,0)*A(0,1))*invdet;
            arr[0] = (mat[4] * mat[8] - mat[5] * mat[7]) * inv_det;
            arr[1] = -(mat[3] * mat[8] - mat[6] * mat[5]) * inv_det;
            arr[2] = (mat[3] * mat[7] - mat[6] * mat[4]) * inv_det;
            arr[3] = -(mat[1] * mat[8] - mat[7] * mat[2]) * inv_det;
            arr[4] = (mat[0] * mat[8] - mat[6] * mat[2]) * inv_det;
            arr[5] = -(mat[0] * mat[7] - mat[1] * mat[6]) * inv_det;
            arr[6] = (mat[1] * mat[5] - mat[2] * mat[4]) * inv_det;
            arr[7] = -(mat[0] * mat[5] - mat[2] * mat[3]) * inv_det;
            arr[8] = (mat[0] * mat[4] - mat[1] * mat[3]) * inv_det;

            transpose(arr, 3, 3);
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

        GLfloat MatMatrix::length(const Vec3& v) {
            __m128 a = _mm_set_ps(1.f, v.matrix[2], v.matrix[1], v.matrix[0]);
            return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(a, a, 0x71)));
        }
        GLfloat MatMatrix::dot(
                const Vec3& a, const Vec3& b) {
            return _mm_cvtss_f32(
                    _mm_dp_ps(
                            _mm_set_ps(1.f, a.matrix[2], a.matrix[1],
                                    a.matrix[0]),
                            _mm_set_ps(1.f, b.matrix[2], b.matrix[1],
                                    b.matrix[0]),
                            0x71)
                            );
        }
        Vec4 MatMatrix::cross(const Vec3& a, const Vec3& b) {
            return {
                a.Y() * b.Z() - a.Z() * b.Y(),
                a.Z() * b.X() - a.X() * b.Z(),
                a.X() * b.Y() - a.Y() * b.X(),
                1.f
            };
        }
        GLfloat MatMatrix::distance(const Vec3& a, const Vec3& b) {
            return sqrt((a.X() - b.X()) * (a.X() - b.X()) +
                    (a.Y() - b.Y()) * (a.Y() - b.Y()) +
                    (a.Z() - b.Z()) * (a.Z() - b.Z()));
        }
    }
}

