#include "Types.hpp"
#include "Dimensions.hpp"

namespace GL3Engine {
    /** Point3D */
    template<typename T>
    Point3D<T>::Point3D(const T& _X, const T& _Y, const T& _Z)
            :
              X(_X),
              Y(_Y),
              Z(_Z) {
    }
    template<typename T>
    Point3D<T>::Point3D(const array<T, 3>& pos)
            :
              X(pos[0]),
              Y(pos[1]),
              Z(pos[2]) {
        
    }
    
    template<typename T>
    GLboolean Point3D<T>::operator>(const Point3D<T>& p) const {
        return X + Y + Z > p.X + p.Y + p.Z;
    }
    template<typename T>
    GLboolean Point3D<T>::operator<(const Point3D<T>& p) const {
        return !(*this > p);
    }
    template<typename T>
    Point3D<T> Point3D<T>::operator-() const {
        return Point3D<T> {
                -X, -Y, -Z };
    }
    
    template<typename T>
    void Point3D<T>::copyTo3DArray(T array[3]) {
        std::template array<T, 3> v = toArray();
        copy(v.begin(), v.end(), array);
    }
    template<typename T>
    void Point3D<T>::copyTo2DArray(T array[2]) {
        std::template array<T, 3> v = toArray();
        copy(v.begin(), v.end() - 1, array);
    }
    
    template<typename T>
    Point3D<T>& Point3D<T>::normalize() {
        T length = getVecLength();
        this->X /= length;
        this->Y /= length;
        this->Z /= length;
        return *this;
    }
    
    template class Point3D<GLfloat> ;
    template class Point3D<GLint> ;
    
    /** Point2D */
    template<typename T>
    Point2D<T>::Point2D(const T& _X, const T& _Y)
            :
              Point3D<T>(_X, _Y, 0) {
    }
    template<typename T>
    Point2D<T>::Point2D(const array<T, 2>& pos)
            :
              Point3D<T>(array<T, 3> {
                      pos[0], pos[1], 0 }) {
    }
    
    template class Point2D<GLfloat> ;
    template class Point2D<GLint> ;
}

