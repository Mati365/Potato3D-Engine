#ifndef TYPES_HPP_
#define TYPES_HPP_
#include <initializer_list>

#include "Matrix.hpp"

namespace GL3Engine {
    using namespace std;

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
}

#endif
