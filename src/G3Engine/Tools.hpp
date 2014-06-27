/*
 * Tools.hpp
 *
 *  Created on: 27 cze 2014
 *      Author: mateusz
 */

#ifndef TOOLS_HPP_
#define TOOLS_HPP_
#include <deque>
#include <GL/gl.h>
#include <iostream>

namespace Tools {
    using namespace std;

#define IS_SET_FLAG(num, flag) (num & flag)
#define ARRAY_LENGTH(type, array) (sizeof(array) / sizeof(type))

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

            Point3D<T>& operator+=(const Point3D<T>& v) {
                X += v.X;
                Y += v.Y;
                Z += v.Z;
                return *this;
            }
            Point3D<T>& operator-=(const Point3D<T>& v) {
                X -= v.X;
                Y -= v.Y;
                Z -= v.Z;
                return *this;
            }
            Point3D<T>& operator*=(const Point3D<T>& v) {
                this->X *= v.X;
                this->Y *= v.Y;
                this->Z *= v.Z;
                return *this;
            }

            typedef T (&array)[3];
            operator array() const {
                return {X,Y,Z};
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

    template<typename T> void safe_delete(T*& ptr, bool arr) {
        if (ptr == nullptr)
            return;

        if (arr)
            delete[] ptr;
        else
            delete ptr;
        ptr = nullptr;
    }

    struct Log {
            enum Flag {
                CRITICAL,
                WARNING,
                ERROR
            };

            Flag flag;
            string message;

            static deque<Log> logs;
    };
    void showGLErrors();

    template<typename T> class Singleton {
        public:
            static T& getInstance() {
                static T t;
                return t;
            }
    };
}

#endif /* TOOLS_HPP_ */
