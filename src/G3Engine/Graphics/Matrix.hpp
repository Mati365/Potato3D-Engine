/*
 * Matrix.hpp
 *
 *  Created on: 27 cze 2014
 *      Author: mateusz
 */

#ifndef MATRIX_HPP_
#define MATRIX_HPP_
#include "../Tools.hpp"

namespace Graphics {
    using namespace IO;

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

                safe_delete<GLfloat>(this->matrix, true);

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

                    safe_delete<GLfloat>(this->matrix, true);
                    this->matrix = new T[rows * cols];
                }
                memcpy(this->matrix, matrix.matrix,
                        rows * cols * sizeof(GLfloat));
                return *this;
            }
            Matrix<T>& operator=(const T* dynamic_array) {
                safe_delete<GLfloat>(this->matrix, true);
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
                safe_delete(matrix, true);
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
}

#endif /* MATRIX_HPP_ */
