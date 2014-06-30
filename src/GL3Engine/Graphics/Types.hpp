#ifndef TYPES_HPP_
#define TYPES_HPP_
#include <initializer_list>

#include "Matrix.hpp"

namespace GL3Engine {
    using namespace std;

    struct Color {
            GLfloat col[4] = { 0, 0, 0, 0 };

            Color() {
            }
            Color(const initializer_list<GLfloat>& _col) {
                copy(_col.begin(), _col.end(), col);
            }

            array<GLfloat, 4> toArray() const {
                return {
                    col[0],
                    col[1],
                    col[2],
                    col[3]
                };
            }
            Color& operator=(const FPoint3D& p) {
                col[0] = p.X;
                col[1] = p.Y;
                col[2] = p.Z;
                col[3] = 1.f;
                return *this;
            }

#define RETURN_COLOR(color, var) inline GLfloat color() const { return col[var]; }
            RETURN_COLOR(r, 0)
            RETURN_COLOR(g, 1)
            RETURN_COLOR(b, 2)
            RETURN_COLOR(a, 3)
            // Kolory
    };
    struct Vertex {
            GLfloat pos[3];
            GLfloat normal[3];
            GLfloat uv[2];
            GLuint  mtl;
    };
}

#endif