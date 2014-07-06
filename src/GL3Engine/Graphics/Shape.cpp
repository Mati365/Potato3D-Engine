#include "Mesh.hpp"

#include "Effects.hpp"
#include "../IO.hpp"

namespace GL3Engine {
    using namespace IO;

    GLint genGLBuffer(const GL_BUFFER_DATA& data, bool bind) {
        GLuint buffer = 0;

        glGenBuffers(1, &buffer);
        glBindBuffer(data.type, buffer);
        glBufferData(data.type, data.len, data.data, GL_STATIC_DRAW);
        if (!bind)
            glBindBuffer(data.type, 0);

        return buffer;
    }

#define VRAM_MINIMUM_SIZE 4194304 // 32MB

    void Shape::draw(MatrixStack& matrix, GLint mode) {
        static VBOBath bath(VRAM_MINIMUM_SIZE);

        static Shader shader(
                getFileContents("shaders/fragment_shader.glsl"),
                getFileContents("shaders/vertex_shader.glsl"),
                "");

        shader.begin();
        shader.setUniform("matrix.mvp", matrix.vp_matrix * matrix.model);
        shader.setUniform("matrix.normal",
                FMAT_MATH::inverse(matrix.model.getCut(3, 3)));
        if (materials.empty())
            shader.setUniform("col", col);
        else
            shader.setUniform("material", materials);
        {
            glBindVertexArray(vao);
            if (!indices)
                glDrawArrays(mode, 0, vertices_count);
            else
                glDrawElements(mode, indices_count, GL_UNSIGNED_SHORT,
                        nullptr);
            glBindVertexArray(0);
        }
        shader.end();
    }
    void Shape::create(const Vertex* buffer, GLint vertices,
            const GLushort* i_buffer,
            GLint indices) {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Generowani bufora indeksow
        if (i_buffer != nullptr) {
            this->indices = genGLBuffer(GL_BUFFER_DATA(
                    i_buffer,
                    indices,
                    GL_ELEMENT_ARRAY_BUFFER
                    ), true);
            this->indices_count = indices;
        }

        // Generowanie bufora wierzcholkow
        vbo = genGLBuffer( {
                buffer,
                static_cast<GLsizeiptr>(vertices * sizeof(Vertex)),
                GL_ARRAY_BUFFER
        }, true);
        this->vertices_count = vertices;

#define BUFFER_OFFSET(i) ((char *)nullptr + (i))
#define VERTEX_ATTR_PTR(loc, count, strip) \
        glVertexAttribPointer(loc, count, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(strip * sizeof(GLfloat))); \
        glEnableVertexAttribArray(loc)

        VERTEX_ATTR_PTR(0, 3, 0); // Vertex
        VERTEX_ATTR_PTR(1, 3, 3); // Normals
        VERTEX_ATTR_PTR(2, 2, 6); // UVs
        VERTEX_ATTR_PTR(3, 1, 8); // MTL

        glBindVertexArray(0);
    }
}

