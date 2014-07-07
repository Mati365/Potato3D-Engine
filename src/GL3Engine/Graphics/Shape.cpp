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

    void Shape::draw(MatrixStack& matrix, GLint mode) {
        static Shader shader(
                getFileContents("shaders/fragment_shader.glsl"),
                getFileContents("shaders/vertex_shader.glsl"),
                "");

        shader.begin();
        shader.setUniform("matrix.mvp", matrix.vp_matrix * matrix.model);
        shader.setUniform("matrix.m", matrix.model);
        shader.setUniform("matrix.normal",
                FMAT_MATH::inverse(matrix.model.getCut(3, 3)));
        shader.setUniform("matrix.cam", matrix.getActiveCamera()->pos);

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
    void Shape::create(const GL_BUFFER_DATA& vertices,
            const GL_BUFFER_DATA& indices) {
        // Generowanie osobnego VBO dla obiektu
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Generowani bufora indeksow
        if (indices.data != nullptr) {
            this->indices = genGLBuffer(indices, true);
            this->indices_count = indices.len / sizeof(GLushort);
        }

        // Generowanie bufora wierzcholkow
        vbo = genGLBuffer(vertices, true);
        this->vertices_count = vertices.len / sizeof(Vertex);

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
