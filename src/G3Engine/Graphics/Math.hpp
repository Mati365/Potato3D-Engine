/*
 * Math.hpp
 *
 *  Created on: 27 cze 2014
 *      Author: mateusz
 */

#ifndef MATH_HPP_
#define MATH_HPP_
#include "Matrix.hpp"

namespace Graphics {
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
    };

    using FMAT_MATH = Graphics::MatMatrix<GLfloat>;
    using IMAT_MATH = Graphics::MatMatrix<GLint>;
}

#endif /* MATH_HPP_ */
