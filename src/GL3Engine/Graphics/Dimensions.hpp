#ifndef DIMENSIONS_HPP_
#define DIMENSIONS_HPP_
#include <array>

namespace GL3Engine {
    using namespace std;

    /** Wektory nie są macierzami ze względów wyadjnosci */
    template<typename T> class Point3D {
        public:
            T X = 0,
                    Y = 0,
                    Z = 0;

            Point3D() {
            }
            Point3D(const T&, const T&, const T&);
            Point3D(const array<T, 3>&);

#define POINT3D_OPERATOR(oper) \
            Point3D<T>& operator oper (const Point3D<T>& v) { \
                X oper v.X; \
                Y oper v.Y; \
                Z oper v.Z; \
                return *this; \
            }
            POINT3D_OPERATOR(+=)
            POINT3D_OPERATOR(-=)
            POINT3D_OPERATOR(*=)
            POINT3D_OPERATOR(/=)

            template<typename C> operator Point3D<C>() const {
                return Point3D<C>(
                        this->X,
                        this->Y);
            }

            GLboolean operator>(const Point3D<T>&) const;
            GLboolean operator<(const Point3D<T>&) const;

            inline array<T, 3> toArray() {
                return {X, Y, Z};
            }
            void copyTo3DArray(T array[3]);
            void copyTo2DArray(T array[2]);

            T operator[](GLint i) const {
                return i == 0 ? X : (i == 1 ? Y : Z);
            }
            Point3D<T> operator-() const;

            inline T getVecLength() const {
                return sqrt(pow(this->X, 2) + pow(this->Y, 2) + pow(this->Z, 2));
            }
            Point3D<T>& normalize();

            virtual ~Point3D() {
            }
        };

    extern template class Point3D<GLfloat> ;
    extern template class Point3D<GLint> ;

#define VEC_OVERLOAD(oper) \
    template<typename T> Point3D<T> operator oper (const Point3D<T>& l, \
            const Point3D<T>& p) { \
        Point3D<T> _l = l; \
        _l oper##= p; \
        return _l; \
    }
    VEC_OVERLOAD(+)
    VEC_OVERLOAD(-)
    VEC_OVERLOAD(*)

    template<typename T> class Point2D : public Point3D<T> {
        public:
            Point2D() {
            }
            Point2D(const T&, const T&);
            Point2D(const array<T, 2>&);

            template<typename C> operator Point2D<C>() const {
                return Point2D<C>(
                        this->X,
                        this->Y
                        );
            }
            inline array<T, 2> toArray() {
                return {this->X, this->Y};
            }
    };

    extern template class Point2D<GLfloat> ;
    extern template class Point2D<GLint> ;

    /** Operacje na wektorach */
    template<typename T> inline Point3D<T> normalize(const Point3D<T>& _p) {
        return Point3D<T>(_p).normalize();
    }
    template<typename T> Point3D<T> cross(const Point3D<T>& a,
            const Point3D<T>& b) {
        return {
            a.Y * b.Z - a.Z * b.Y,
            a.Z * b.X - a.X * b.Z,
            a.X * b.Y - a.Y * b.X
        };
    }
    template<typename T> inline T dot(const Point3D<T>& a,
            const Point3D<T>& b) {
        return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
    }

    using IPoint2D = Point2D<GLint>;
    using FPoint2D = Point2D<GLfloat>;
    using FPoint3D = Point3D<GLfloat>;

#define BOX_DEFINE(dimensions) \
    template<typename T> class Box##dimensions##D { \
        public: \
            Point##dimensions##D<T> pos, \
                    size; \
            Box##dimensions##D() {} \
            Box##dimensions##D(const Point##dimensions##D<T>& _pos, const Point##dimensions##D<T>& _size) \
                    : \
                      pos(_pos), \
                      size(_size) { \
            } \
    }
    BOX_DEFINE(2);
    BOX_DEFINE(3);
}

#endif
