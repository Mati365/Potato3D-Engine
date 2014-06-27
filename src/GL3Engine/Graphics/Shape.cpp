#include "Mesh.hpp"

#include "Effects.hpp"
#include "../IO.hpp"

namespace GL3Engine {
    using namespace IO;

    void Shape::draw(MatrixStack& matrix, GLint mode) {
        static Shader shader(
                getFileContents("shaders/fragment_shader.txt"),
                getFileContents("shaders/vertex_shader.txt"),
                "");

        shader.begin();
        shader.setUniform("mvp", matrix.vp_matrix * matrix.model);
        shader.setUniform("col", { col.r, col.g, col.b, col.a }, 4);
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
            this->indices = genGLBuffer<GLushort>(
                    i_buffer,
                    indices,
                    GL_ELEMENT_ARRAY_BUFFER);
            this->indices_count = indices;
        }

        // Generowanie bufora wierzcholkow
        vbo = genGLBuffer<Vertex>(
                buffer,
                vertices * sizeof(Vertex),
                GL_ARRAY_BUFFER);
        this->vertices_count = vertices;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define VERTEX_ATTR_PTR(loc, count, strip) \
        glVertexAttribPointer(loc, count, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(strip * sizeof(GLfloat))); \
        glEnableVertexAttribArray(loc)

        VERTEX_ATTR_PTR(0, 3, 0); // Vertex
        VERTEX_ATTR_PTR(1, 3, 3); // Normals
        VERTEX_ATTR_PTR(2, 2, 7); // UVs

        glBindVertexArray(0);
    }
}

