#ifndef MATRIX_HPP_
#define MATRIX_HPP_
#include <string.h>
#include <list>
#include <cmath>
#include <array>
#include <utility>
#include <GL/glew.h>

#include "../Tools.hpp"

namespace GL3Engine {
    using namespace std;
    using namespace Tools;
    
    /** Matrix ROW MAJOR! */
    template<typename T> class Matrix {
        public:
            T* matrix = nullptr;
            GLuint cols = 0, rows = 0;

            Matrix(const Matrix<T>& matrix) {
                *this = matrix;
            }
            Matrix(GLuint, GLuint);
            Matrix(GLuint, GLuint, const initializer_list<T>&);

#ifdef DEBUG
            void print() const;
            #endif

            /** Operacje na macierzach */
            Matrix<T>& operator*=(T);
            Matrix<T>& operator*=(const Matrix<T>&);

            Matrix<T>& operator+=(const Matrix<T>&);
            Matrix<T>& operator-=(const Matrix<T>&);

            Matrix<T>& operator=(const Matrix<T>&);
            Matrix<T>& operator=(const T*);

            Matrix<T> operator-() const;
            Matrix<T> getCut(GLuint, GLuint);

            inline void copyTo(T* array, GLuint len) {
                memcpy(array, matrix, len * sizeof(T));
            }

            T get(GLuint index) const {
                return matrix[index];
            }
            T get(GLuint x, GLuint y) const {
                return matrix[y * cols + x];
            }
            Matrix<T>& set(GLuint x, GLuint y, T val) {
                matrix[y * cols + x] = val;
                return *this;
            }
            T& operator[](GLuint i) {
                return matrix[i];
            }

            GLint getLength() const {
                return rows * cols;
            }
            
            ~Matrix() {
                safeDelete(matrix, true);
            }
    };
    extern template class Matrix<GLfloat> ;
    extern template class Matrix<GLint> ;
    
    using fMat = Matrix<GLfloat>;

#define DEF_EXTERN_MAT_OPER(oper) \
    template<typename T> inline Matrix<T> operator oper(const Matrix<T>& l, \
                                             const Matrix<T>& r) { \
        Matrix<T> temp = l; \
        temp oper##= r; \
        return temp; \
    }
    DEF_EXTERN_MAT_OPER(-)
    DEF_EXTERN_MAT_OPER(+)
    DEF_EXTERN_MAT_OPER(*)
    
    template<typename T> inline Matrix<T> operator *(const Matrix<T>& l,
            T val) {
        Matrix<T> temp = l;
        temp *= val;
        return temp;
    }
    template<typename T, GLuint COLS, GLuint ROWS> class t_Matrix :
                                                                    public Matrix<
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
            t_Matrix(const T* array)
                    :
                      Matrix<T>(COLS, ROWS) {
                memcpy(this->matrix, array, ROWS * COLS * sizeof(T));
            }
            
            t_Matrix<T, COLS, ROWS>& operator=(const Matrix<T>& matrix) {
                Matrix<T>::operator =(matrix);
                return *this;
            }

#define VEC_GETTER(name, index) \
            T name() const { return this->matrix[index]; } \
            t_Matrix<T, COLS, ROWS>& set##name(T obj) { this->matrix[index] = obj; return *this; }

            VEC_GETTER(X, 0)
            VEC_GETTER(Y, 1)
            VEC_GETTER(Z, 2)
            VEC_GETTER(W, 3)

            static constexpr GLuint index(GLuint x, GLuint y) {
                return y * COLS + x;
            }
    };
    
    using Mat4 = t_Matrix<GLfloat, 4, 4>;
    using Mat3 = t_Matrix<GLfloat, 3, 3>;
    using Mat2 = t_Matrix<GLfloat, 2, 2>;
    
    using Vec4 = t_Matrix<GLfloat, 1, 4>;
    using Vec3 = t_Matrix<GLfloat, 1, 3>;
    using Vec2 = t_Matrix<GLfloat, 1, 2>;
    
    using Vec2i = t_Matrix<GLint, 1, 2>;
    using Vec3i = t_Matrix<GLint, 1, 3>;
    using Vec4i = t_Matrix<GLint, 1, 4>;

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
                        1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 });
            }
            static void identity(GLuint, ...);

            static const Mat4& translate(const Vec3&);
            static void translate(fMat&, const Vec3&);

            static const Mat4& scale(const Vec3&);
            static void scale(fMat&, const Vec3&);

            static const Mat4& rotate(GLfloat, const Vec3&);
            static void rotate(fMat&, GLfloat, const Vec3&);

            /**
             * Obiliczenia dla MVP
             * Wzory:
             * http://www.3dcpptutorials.sk/index.php?id=2
             */
            static Mat4 perspective(GLfloat, GLfloat, GLfloat, GLfloat);
            static Mat4 lookAt(const Vec4&, const Vec4&, const Vec4&);
            static Mat4 orthof(const array<Vec2, 3>&);

            /** Obliczenia dla normal matrix */
            static void transpose(GLfloat*, GLuint, GLuint);
            static fMat transpose(const fMat&);

            static void inverse(Mat3*);
            static Mat3 inverse(const Mat3&);

            static inline GLfloat length(const Vec3& v) {
                return sqrt(v.X() * v.X() + v.Y() * v.Y() + v.Z() * v.Z());
            }
            static GLfloat distance(const Vec3&, const Vec3&);

            static void normalize(Vec4&);
            static Vec4 normalize(const Vec4&);

            static Vec4 cross(const Vec3&, const Vec3&);
            static GLfloat dot(const Vec3&, const Vec3&);

            static inline void mul(Vec4& v, const Mat4& m) {
                v = m * v;
            }
            static inline void mul(vector<Vec4*> v, const Mat4& m) {
                for (auto& _v : v)
                    *_v = m * (*_v);
            }

            static void calcW(Vec4&);
            static Vec4 calcW(const Vec4&);
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
