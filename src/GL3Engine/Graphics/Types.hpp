#ifndef TYPES_HPP_
#define TYPES_HPP_
#include "Matrix.hpp"

namespace GL3Engine {
    struct Color {
            GLfloat r = 0.f,
                    g = 0.f,
                    b = 0.f,
                    a = 1.f;

            Color() {
            }
            Color(const GLfloat& _r, const GLfloat& _g, const GLfloat& _b,
                    const GLfloat& _a)
                    :
                      r(_r),
                      g(_g),
                      b(_b),
                      a(_a) {
            }
    };
    struct Vertex {
            GLfloat pos[3];
            GLfloat normal[3];
            GLfloat uv[2];
    };
    struct Camera {
            Vec4 pos = { 0.f, 0.f, 2.5f, 1.f };
            Vec4 target = { 0.f, 0.f, 0.f, 1.f };
    };
}

#endif
