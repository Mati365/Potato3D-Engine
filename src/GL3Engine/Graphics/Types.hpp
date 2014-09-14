#ifndef TYPES_HPP_
#define TYPES_HPP_
#include <initializer_list>

#include "Matrix.hpp"

namespace GL3Engine {
    namespace CoreType {
        struct BufferData {
                const GLvoid* data;
                size_t len;
                GLint type;

                GLuint offset;
                GLuint vbo_draw_type;
        };
        extern GLint genGLBuffer(
                const CoreType::BufferData&,
                GLboolean bind = false);

        using COL = GLfloat[4];
        using UV = GLfloat[2];
        using NORMAL = GLfloat[3];
        using POS = NORMAL;

        struct Vertex2f {
                POS pos;
                UV uv;
        };
        struct Vertex4f {
                POS pos;
                NORMAL normal;
                UV uv;
                GLint mtl; // domyślnie solid color wyłączony!
                POS tangent;
        };

        using Color = CoreMatrix::Vec4;
        using Cube = Vertex4f[36];
        using VertexArray = std::vector<Vertex4f>;

        extern void calcTangents(VertexArray&);
        extern void calcTangents(const std::array<Vertex4f*, 3>&);
    }

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))
#define VERTEX_ATTR_PTR(loc, count, strip, type) \
        glVertexAttribPointer(loc, count, GL_FLOAT, GL_FALSE, sizeof(type), BUFFER_OFFSET(strip * sizeof(GLfloat))); \
        glEnableVertexAttribArray(loc)
}

#endif
