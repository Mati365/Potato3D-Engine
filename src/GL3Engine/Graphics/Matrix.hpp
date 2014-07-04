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
            Matrix(GLuint _cols, GLuint _rows)
                    :
                      cols(_cols),
                      rows(_rows) {
                matrix = new T[rows * cols];
                memset(matrix, 0, rows * cols * sizeof(GLfloat));
            }
            Matrix(GLuint _cols, GLuint _rows, const initializer_list<T>& array)
                    :
                      Matrix<T>(_cols, _rows) {
                for (GLuint i = 0; i < array.size(); ++i)
                    matrix[i] = *(array.begin() + i);
            }
            void print() const {
                for (GLuint i = 0; i < rows; ++i) {
                    for (GLuint j = 0; j < cols; ++j)
                        cout << matrix[i * cols + j] << " ";
                    cout << endl;
                }
            }

            /** Operacje na macierzach */
            Matrix<T>& operator*=(T k) {
                for (GLuint i = 0; i < rows * cols; ++i)
                    matrix[i] *= k;
                return *this;
            }
            Matrix<T>& operator*=(const Matrix<T>& matrix) {
                T* temp = new T[rows * matrix.cols];
                for (GLuint i = 0; i < matrix.cols; ++i)
                    for (GLuint j = 0; j < rows; ++j) {
                        T sum = 0;
                        for (GLuint k = 0; k < cols; ++k)
                            sum += this->matrix[j * cols + k]
                                    * matrix.matrix[k * matrix.cols + i];
                        temp[j * matrix.cols + i] = sum;
                    }

                safeDelete<GLfloat>(this->matrix, true);

                cols = matrix.cols;
                this->matrix = temp;

                return *this;
            }
            Matrix<T>& operator+=(const Matrix<T>& matrix) {
                for (GLuint i = 0; i < rows; ++i)
                    for (GLuint j = 0, index = 0; j < cols;
                            ++j, index = i * cols + j)
                        this->matrix[index] += *matrix.matrix[index];
                return *this;
            }
            Matrix<T>& operator=(const Matrix<T>& matrix) {
                if (rows != matrix.rows && cols != matrix.cols) {
                    rows = matrix.rows;
                    cols = matrix.cols;

                    safeDelete<GLfloat>(this->matrix, true);
                    this->matrix = new T[rows * cols];
                }
                memcpy(this->matrix, matrix.matrix,
                        rows * cols * sizeof(GLfloat));
                return *this;
            }
            Matrix<T>& operator=(const T* dynamic_array) {
                safeDelete<GLfloat>(this->matrix, true);
                this->matrix = dynamic_array;
                return *this;
            }

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

            T* get(GLuint x, GLuint y) {
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
            Vec4 pos = { 2.f, 1.f, 2.5f, 1.f };
            Vec4 target = { 0.f, 0.f, 0.f, 1.f };
    };
    class MatrixStack {
        public:
            Mat4 projection,
                    view,
                    model,
                    vp_matrix; // cache z mnożenia view * projection
            Camera cam;

            struct M_STACK_ARRAY {
                    GLfloat array[16];
            };
            list<M_STACK_ARRAY> stack;

            MatrixStack();
            void updateCameraCoords();

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
            static inline void identity(Matrix<T>& matrix) {
                matrix = identity();
            }

            static Mat4 translate(const FPoint3D& v) {
                return Mat4( {
                        1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        v.X, v.Y, v.Z, 1
                });
            }
            static inline void translate(Matrix<T>& matrix,
                    const FPoint3D& v) {
                matrix *= translate(v);
            }

            static Mat4 scale(const FPoint3D& scale) {
                return Mat4( {
                        scale.X, 0, 0, 0,
                        0, scale.Y, 0, 0,
                        0, 0, scale.Z, 0,
                        0, 0, 0, 1
                });
            }
            static inline void scale(Matrix<T>& matrix,
                    const FPoint3D& _scale) {
                matrix *= scale(_scale);
            }

            static Mat4 rotate(GLfloat theta,
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
            static inline void rotate(Matrix<T>& matrix, GLfloat theta,
                    const FPoint3D& axis) {
                matrix *= rotate(theta, axis);
            }

            /**
             * Obiliczenia dla MVP
             * Wzory:
             * http://www.3dcpptutorials.sk/index.php?id=2
             */
            static Mat4 perspective(GLfloat angle, GLfloat aspect, GLfloat near,
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
            static Mat4 lookAt(const FPoint3D& _eye,
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

            /** Obliczenia dla normal matrix */
            static void transpose(const Matrix<T>& matrix, T* array) {
                for (GLint i = 0; i < matrix.cols; ++i)
                    for (GLint j = 0; j < matrix.rows; ++j)
                        array[i * matrix.rows + j] = matrix.matrix[j
                                * matrix.cols + i];
            }
            static Matrix<T> transpose(const Matrix<T>& matrix) {
                Matrix<T> result(matrix.rows, matrix.cols);
                transpose(matrix, result.matrix);
                return result;
            }

            static void inverse(Mat3* matrix) {
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
            static Mat3 inverse(const Mat3& matrix) {
                Mat3 mat(matrix);
                inverse(&mat);
                return mat;
            }
    };

    using FMAT_MATH = MatMatrix<GLfloat>;
    using IMAT_MATH = MatMatrix<GLint>;
}

#endif
