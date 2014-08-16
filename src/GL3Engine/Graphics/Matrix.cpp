#include "Matrix.hpp"

namespace GL3Engine {
    template<typename T>
    Matrix<T>::Matrix(GLuint _cols, GLuint _rows)
            :
              cols(_cols),
              rows(_rows) {
        matrix = new T[rows * cols];
        memset(matrix, 0, rows * cols * sizeof(GLfloat));
    }
    template<typename T>
    Matrix<T>::Matrix(
            GLuint _cols, GLuint _rows, const initializer_list<T>& array)
            :
              Matrix<T>::Matrix<T>(_cols, _rows) {
        for (GLuint i = 0; i < array.size(); ++i)
            matrix[i] = *(array.begin() + i);
    }

#ifdef DEBUG
    template<typename T>
    void Matrix<T>::print() const {
        for (GLuint i = 0; i < rows; ++i) {
            for (GLuint j = 0; j < cols; ++j)
                cout << matrix[i * cols + j] << " ";
            cout << endl;
        }
    }
#endif
    
    /** OPERATORY */
    template<typename T>
    Matrix<T>& Matrix<T>::operator*=(T k) {
        for (GLuint i = 0; i < rows * cols; ++i)
            matrix[i] *= k;
        return *this;
    }
    template<typename T>
    Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& matrix) {
        T* temp = new T[rows * matrix.cols];
        for (GLuint i = 0; i < matrix.cols; ++i)
            for (GLuint j = 0; j < rows; ++j) {
                T sum = 0;
                for (GLuint k = 0; k < cols; ++k)
                    sum += this->matrix[j * cols + k]
                            * matrix.matrix[k * matrix.cols + i];
                temp[j * matrix.cols + i] = sum;
            }
        cols = matrix.cols;
        
        safeDelete(this->matrix, true);
        this->matrix = temp;
        
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
    template<typename T>
    Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& matrix) {
        return addMat(*this, matrix, 1.f);
    }
    template<typename T>
    Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& matrix) {
        return addMat(*this, matrix, -1.f);
    }
    template<typename T>
    Matrix<T> Matrix<T>::operator-() const {
        Matrix<T> mat(cols, rows);
        for (GLuint i = 0; i < cols * rows; ++i)
            mat[i] = -matrix[i];
        return mat;
    }
    
    template<typename T>
    Matrix<T>& Matrix<T>::operator=(const Matrix<T>& matrix) {
        if (rows != matrix.rows || cols != matrix.cols) {
            rows = matrix.rows;
            cols = matrix.cols;
            
            safeDelete(this->matrix, true);
            this->matrix = new T[rows * cols];
        }
        memcpy(this->matrix, matrix.matrix, rows * cols * sizeof(GLfloat));
        return *this;
    }
    template<typename T>
    Matrix<T>& Matrix<T>::operator=(T* dynamic_array) {
        safeDelete(this->matrix, true);
        this->matrix = dynamic_array;
        return *this;
    }
    
    template<typename T>
    Matrix<T> Matrix<T>::getCut(GLuint cols, GLuint rows) {
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

