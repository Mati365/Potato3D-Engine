#ifndef MATRIX_HPP_
#define MATRIX_HPP_
#include <string.h>
#include <list>
#include <math.h>

#include "../Tools.hpp"
#include "Dimensions.hpp"

namespace GL3Engine {
    using namespace std;
    using namespace Tools;

    /** Matrix */
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

    using Vec4 = t_Matrix<GLfloat, 4, 1>;
    using Vec3 = t_Matrix<GLfloat, 3, 1>;
    using Vec2 = t_Matrix<GLfloat, 2, 1>;

    /** Stos */
    struct Camera {
            Vec4 pos;
            Vec4 target;
    };
    class MatrixStack {
        private:
            vector<Camera*> cam;
            GLuint active_cam = 0;

            struct M_STACK_ARRAY {
                    GLfloat array[16];
            };
            list<M_STACK_ARRAY> stack; // pushTransform i popTransform

        public:
            Mat4 projection,
                    view,
                    model,
                    vp_matrix; // cache z mnożenia view * projection

            MatrixStack();
            void updateCameraCoords();

            inline GLuint addCam(Camera* _cam) {
                cam.push_back(_cam);
                return cam.size() - 1;
            }
            inline void selectCam(GLint index) {
                active_cam = index;
                updateCameraCoords();
            }

            inline Camera* getActiveCamera() {
                return cam[active_cam];
            }

            void pushTransform();
            void popTransform();
    };

    /** Obliczenia */
    template<typename T> class MatMatrix {
        public:
            static inline GLfloat toRad(GLfloat deg) {
                return deg * 180.0 / 3.145;
            }
            static inline GLfloat cotan(GLfloat rad) {
                return 1.f / tan(rad);
            }

            /** Operacje na macierzy macierzy [ x, y, z, w ] */
            static constexpr Mat4 identity() {
                return Mat4( {
                        1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1
                });
            }
            static inline void identity(Matrix<T>&);

            static Mat4 translate(const FPoint3D&);
            static inline void translate(Matrix<T>&, const FPoint3D&);

            static Mat4 scale(const FPoint3D&);
            static inline void scale(Matrix<T>&, const FPoint3D&);

            static Mat4 rotate(GLfloat, const FPoint3D&);
            static inline void rotate(Matrix<T>&, GLfloat, const FPoint3D&);

            /**
             * Obiliczenia dla MVP
             * Wzory:
             * http://www.3dcpptutorials.sk/index.php?id=2
             */
            static Mat4 perspective(GLfloat, GLfloat, GLfloat, GLfloat);
            static Mat4 lookAt(const FPoint3D&, const FPoint3D&,
                    const FPoint3D&);

            /** Obliczenia dla normal matrix */
            static void transpose(const Matrix<T>&, T*);
            static Matrix<T> transpose(const Matrix<T>&);

            static void inverse(Mat3*);
            static Mat3 inverse(const Mat3&);
    };

    using FMAT_MATH = MatMatrix<GLfloat>;
}

#endif
