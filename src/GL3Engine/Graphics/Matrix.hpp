#ifndef MATRIX_HPP_
#define MATRIX_HPP_
#include <string.h>
#include <list>
#include <math.h>
#include <array>
#include <utility>
#include <GL/glew.h>
#include <GL/glu.h>

#include "../Tools.hpp"
#include "Dimensions.hpp"
#include "ECS.hpp"

namespace GL3Engine {
    using namespace std;
    using namespace Tools;

    /** Matrix ROW MAJOR! */
    template<typename T> class Matrix {
        public:
            T* matrix = nullptr;
            GLuint cols = 0,
                    rows = 0;

            Matrix(const Matrix<T>& matrix) {
                *this = matrix;
            }
            Matrix(GLuint, GLuint);
            Matrix(GLuint, GLuint, const initializer_list<T>&);

            void print() const;

            /** Operacje na macierzach */
            Matrix<T>& operator*=(T);
            Matrix<T>& operator*=(const Matrix<T>&);

            Matrix<T>& operator+=(const Matrix<T>&);
            Matrix<T>& operator-=(const Matrix<T>&);

            Matrix<T>& operator=(const Matrix<T>&);
            Matrix<T>& operator=(T*);

            operator Point3D<T>() const {
                return {
                    matrix[0],
                    matrix[1],
                    matrix[2]
                };
            }
            operator Point2D<T>() const {
                return {
                    matrix[0],
                    matrix[1]
                };
            }

            Matrix<T> getCut(GLuint, GLuint);

            T get(GLuint x, GLuint y) {
                return matrix[y * cols + x];
            }
            T& operator[](GLuint i) {
                return matrix[i];
            }
            inline GLint getLength() const {
                return rows * cols;
            }

            ~Matrix() {
                safeDelete(matrix, true);
            }
    };
    extern template class Matrix<GLfloat> ;

    using FMat = Matrix<GLfloat>;

    template<typename T> Matrix<T> operator*(const Matrix<T>& l,
                                             const Matrix<T>& r) {
        Matrix<T> temp = l;
        temp *= r;
        return temp;
    }
    template<typename T, GLuint COLS, GLuint ROWS> class t_Matrix : public Matrix<
            T> {
        public:
            t_Matrix()
                    :
                      Matrix<T>(COLS, ROWS) {
            }
            t_Matrix(const Matrix<T>& matrix)
                    :
                      Matrix<T>(matrix) {
            }
            t_Matrix(const initializer_list<T>& array)
                    :
                      Matrix<T>(COLS, ROWS, array) {
            }

            t_Matrix<T, COLS, ROWS>& operator=(const Matrix<T>& matrix) {
                Matrix<T>::operator =(matrix);
                return *this;
            }
    };

    using Mat4 = t_Matrix<GLfloat, 4, 4>;
    using Mat3 = t_Matrix<GLfloat, 3, 3>;
    using Mat2 = t_Matrix<GLfloat, 2, 2>;

    using Vec4 = t_Matrix<GLfloat, 1, 4>;
    using Vec3 = t_Matrix<GLfloat, 1, 3>;
    using Vec2 = t_Matrix<GLfloat, 1, 2>;

    /** Obliczenia */
    class MatMatrix {
        public:
            static inline GLfloat toRad(GLfloat deg) {
                return deg * 180.0 / 3.145;
            }
            static inline GLfloat cotan(GLfloat rad) {
                return 1.f / tan(rad);
            }

            /** Operacje na macierzy macierzy [ x, y, z, w ] */
            static Mat4 identity() {
                return Mat4( {
                        1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1
                });
            }
            static void identity(GLuint, ...);

            static const Mat4& translate(const FPoint3D&);
            static void translate(FMat&, const FPoint3D&);

            static const Mat4& scale(const FPoint3D&);
            static void scale(FMat&, const FPoint3D&);

            static const Mat4& rotate(GLfloat, const FPoint3D&);
            static void rotate(FMat&, GLfloat, const FPoint3D&);

            /**
             * Obiliczenia dla MVP
             * Wzory:
             * http://www.3dcpptutorials.sk/index.php?id=2
             */
            static Mat4 perspective(GLfloat, GLfloat, GLfloat, GLfloat);
            static Mat4 lookAt(const FPoint3D&, const FPoint3D&,
                               const FPoint3D&);
            static Mat4 orthof(const array<FPoint2D, 3>&);

            /** Obliczenia dla normal matrix */
            static void transpose(const FMat&, GLfloat*);
            static FMat transpose(const FMat&);

            static void inverse(Mat3*);
            static Mat3 inverse(const Mat3&);

            /** Row Major */
            static inline void mul(Vec4& v, const Mat4& m) {
                v = m * v;
            }
    };
    class Transform {
        public:
            Mat4 model = MatMatrix::identity();

            Transform& mul(const Mat4& m) {
                model *= m;
                return *this;
            }
            Transform& identity() {
                MatMatrix::identity(1, &model);
                return *this;
            }
    };
}

#endif
