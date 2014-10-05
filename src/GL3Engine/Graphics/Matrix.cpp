#include "Matrix.hpp"
#include "AttribContainer.hpp"

namespace GL3Engine {
    namespace CoreMatrix {
        template<typename T> Matrix<T>::Matrix(GLuint _cols, GLuint _rows)
                :
                  cols(_cols),
                  rows(_rows) {
            matrix = new T[rows * cols];
            memset(matrix, 0, rows * cols * sizeof(GLfloat));
        }
        template<typename T> Matrix<T>::Matrix(
                GLuint _cols, GLuint _rows,
                const std::initializer_list<T>& array)
                :
                  Matrix<T>::Matrix<T>(_cols, _rows) {
            for (GLuint i = 0; i < array.size(); ++i)
                matrix[i] = *(array.begin() + i);
        }

#ifdef DEBUG
        template<typename T> void Matrix<T>::print() const {
            for (GLuint i = 0; i < rows; ++i) {
                for (GLuint j = 0; j < cols; ++j)
                    std::cout << matrix[i * cols + j] << " ";
                std::cout << std::endl;
            }
        }
#endif
        
        /** OPERATORY */
        template<typename T> Matrix<T>& Matrix<T>::operator*=(T k) {
            for (GLuint i = 0; i < rows * cols; ++i)
                matrix[i] *= k;
            return *this;
        }
        template<typename T> Matrix<T>& Matrix<T>::operator*=(
                const Matrix<T>& matrix) {
            if (rows == 4 && cols == 4 &&
                    matrix.cols == 4 && matrix.rows == 4) {
                GLfloat temp[16];
                __m128 row1 = _mm_load_ps((const GLfloat*) &matrix.matrix[0]);
                __m128 row2 = _mm_load_ps((const GLfloat*) &matrix.matrix[4]);
                __m128 row3 = _mm_load_ps((const GLfloat*) &matrix.matrix[8]);
                __m128 row4 = _mm_load_ps((const GLfloat*) &matrix.matrix[12]);
                for (GLint i = 0; i < 4; i++) {
                    __m128 brod1 = _mm_set1_ps(this->matrix[4 * i + 0]);
                    __m128 brod2 = _mm_set1_ps(this->matrix[4 * i + 1]);
                    __m128 brod3 = _mm_set1_ps(this->matrix[4 * i + 2]);
                    __m128 brod4 = _mm_set1_ps(this->matrix[4 * i + 3]);
                    __m128 row = _mm_add_ps(
                            _mm_add_ps(
                                    _mm_mul_ps(brod1, row1),
                                    _mm_mul_ps(brod2, row2)),
                            _mm_add_ps(
                                    _mm_mul_ps(brod3, row3),
                                    _mm_mul_ps(brod4, row4)));
                    _mm_store_ps(&temp[4 * i], row);
                }
                memcpy(this->matrix, temp, 16 * sizeof(GLfloat));
            } else {
                T* temp = new T[rows * matrix.cols];
                for (GLuint i = 0; i < matrix.cols; ++i)
                    for (GLuint j = 0; j < rows; ++j) {
                        // Sumacja
                        T sum = 0;
                        for (GLuint k = 0; k < cols; ++k)
                            sum += this->matrix[j * cols + k]
                                    * matrix.matrix[k * matrix.cols + i];
                        temp[j * matrix.cols + i] = sum;
                    }
                cols = matrix.cols;

                delete[] this->matrix;
                this->matrix = temp;
            }
            return *this;
        }

        template<typename T> Matrix<T>& addMat(
                Matrix<T>& a, const Matrix<T>& b, GLfloat m) {
            for (GLuint i = 0; i < a.rows; ++i)
                for (GLuint j = 0; j < b.cols; ++j) {
                    GLuint index = i * a.cols + j;
                    a.matrix[index] += b.matrix[index] * m;
                }
            return a;
        }
        template<typename T> Matrix<T>& Matrix<T>::operator+=(
                const Matrix<T>& matrix) {
            return addMat(*this, matrix, 1.f);
        }
        template<typename T> Matrix<T>& Matrix<T>::operator-=(
                const Matrix<T>& matrix) {
            return addMat(*this, matrix, -1.f);
        }

        template<typename T> Matrix<T>& addMat(Matrix<T>& a, const T* array,
                GLfloat m) {
            for (GLuint i = 0; i < a.getLength(); ++i)
                a.matrix[i] += array[i] * m;
            return a;
        }
        template<typename T> Matrix<T>& Matrix<T>::operator+=(const T* array) {
            return addMat(*this, array, 1.f);
        }
        template<typename T> Matrix<T>& Matrix<T>::operator-=(const T* array) {
            return addMat(*this, array, -1.f);
        }
        template<typename T> Matrix<T> Matrix<T>::operator-() const {
            Matrix<T> mat(cols, rows);
            for (GLuint i = 0; i < cols * rows; ++i)
                mat[i] = -matrix[i];
            return mat;
        }

        template<typename T> Matrix<T>& Matrix<T>::operator=(
                const Matrix<T>& matrix) {
            if (rows != matrix.rows || cols != matrix.cols) {
                rows = matrix.rows;
                cols = matrix.cols;

                Tools::safeDelete(this->matrix, true);
                this->matrix = new T[rows * cols];
            }
            memcpy(this->matrix, matrix.matrix, rows * cols * sizeof(GLfloat));
            return *this;
        }
        template<typename T> Matrix<T>& Matrix<T>::operator=(const T* array) {
            memcpy(matrix, array, rows * cols * sizeof(GLfloat));
            return *this;
        }
        
        template<typename T> Matrix<T> Matrix<T>::getCut(GLuint cols,
                GLuint rows) {
            if (cols > this->cols || rows > this->rows)
                return *this;

            Matrix<T> cut(cols, rows);
            for (GLuint i = 0; i < this->rows && i < rows; ++i)
                for (GLuint j = 0; j < this->cols && j < cols; ++j)
                    cut.matrix[i * rows + j] = matrix[i * this->rows + j];

            return cut;
        }
        
        template class Matrix<GLfloat> ;
        template class Matrix<GLint> ;
    }
}

