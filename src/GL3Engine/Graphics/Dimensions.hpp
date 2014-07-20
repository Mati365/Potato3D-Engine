#ifndef DIMENSIONS_HPP_
#define DIMENSIONS_HPP_
#include <array>

namespace GL3Engine {
    using namespace std;

    template<typename T> class Point3D {
        public:
            T X = 0,
                    Y = 0,
                    Z = 0;

            Point3D() {
            }
            Point3D(const T& _X, const T& _Y, const T& _Z)
                    :
                      X(_X),
                      Y(_Y),
                      Z(_Z) {
            }
            Point3D(const array<T, 3>& pos)
                    :
                      X(pos[0]),
                      Y(pos[1]),
                      Z(pos[2]) {

            }

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

            GLboolean operator>(const Point3D<T>& p) {
                return X + Y + Z > p.X + p.Y + p.Z;
            }
            GLboolean operator<(const Point3D<T>& p) {
                return !(this > p);
            }

            inline array<T, 3> toArray() {
                return {X, Y, Z};
            }
            void copyTo3DArray(T array[3]) {
                std::template array<T, 3> v = toArray();
                copy(v.begin(), v.end(), array);
            }
            void copyTo2DArray(T array[2]) {
                std::template array<T, 3> v = toArray();
                copy(v.begin(), v.end() - 1, array);
            }

            T operator[](GLint i) const {
                return i == 0 ? X : (i == 1 ? Y : Z);
            }
            Point3D<T> operator-() const {
                return Point3D<T> {
                        -X,
                        -Y,
                        -Z
                };
            }

            inline T getVecLength() const {
                return sqrt(pow(this->X, 2) + pow(this->Y, 2) + pow(this->Z, 2));
            }
            Point3D<T>& normalize() {
                T length = getVecLength();
                this->X /= length;
                this->Y /= length;
                this->Z /= length;
                return *this;
            }

            virtual ~Point3D() {
            }
    };

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
    template<typename T> class Point2D : public Point3D<T> {
        public:
            Point2D() {
            }
            Point2D(const T& _X, const T& _Y)
                    :
                      Point3D<T>(_X, _Y, 0) {
            }
            Point2D(const array<T, 2>& pos)
                    :
                      Point3D<T>(array<T, 2> { pos[0], pos[1] }) {
            }

            inline array<T, 2> toArray() {
                return {this->X, this->Y};
            }
    };

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
