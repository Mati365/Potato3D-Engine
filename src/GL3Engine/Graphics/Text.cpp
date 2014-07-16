#include "Text.hpp"

#include "../IO.hpp"
#include "../Resources/Resources.hpp"

namespace GL3Engine {
    using namespace IO;

    // ---------- TextRenderer
    void TextRenderer::setText(const string& text) {
        if (!font)
            return;

        vector<Vertex2f> vertex_buffer;
        vector<GLuint> index_buffer;

        FPoint2D cursor(0.f, 0.f);
        FPoint2D cell_size = font->getCellSize();

        for (char c : text) {
            if (c == '\n') {
                cursor.X = 0.f;
                cursor.Y -= cell_size.Y;
                continue;
            }
            TILE_ITER iter = font->getCharacter(c);
            {
                // UV
                for (GLuint i = 0; i < 4; ++i) {
                    TILE_ITER _v = iter + i;
                    vertex_buffer.push_back( {
                            {
                                    _v->pos[0] + cursor.X,
                                    _v->pos[1] + cursor.Y,
                                    _v->pos[2]
                            },
                            { _v->uv[0], _v->uv[1] }
                    });
                }
                // Index
                for (GLuint i = 0; i < 6; ++i)
                    index_buffer.push_back(
                            Tile::indices[i] + vertex_buffer.size() - 4);
            }
            cursor.X += cell_size.X;
        }
        shape->changeData(
                {
                        &vertex_buffer[0],
                        vertex_buffer.size() * sizeof(Vertex2f),
                        GL_ARRAY_BUFFER,
                        0,
                        GL_DYNAMIC_DRAW
                },
                {
                        &index_buffer[0],
                        index_buffer.size() * sizeof(GLuint),
                        GL_ELEMENT_ARRAY_BUFFER,
                        0,
                        GL_DYNAMIC_DRAW
                });
    }
    void TextRenderer::create() {
        effect = REQUIRE_SHADER(DEFAULT_MESH_SHADER);
        shape = new Shape2D(
                {
                        nullptr,
                        BUFFER_SIZE * sizeof(Vertex2f),
                        GL_ARRAY_BUFFER,
                        0,
                        GL_DYNAMIC_DRAW
                },
                {
                        nullptr,
                        BUFFER_SIZE * sizeof(GLuint),
                        GL_ELEMENT_ARRAY_BUFFER,
                        0,
                        GL_DYNAMIC_DRAW
                },
                col);

        setText("Potato Engine 3D\nTest");
    }

    void TextRenderer::draw(MatrixStack& matrix, GLint) {
        if (!font || !effect || !shape)
            return;

        effect->begin();
        effect->setUniform("matrix.mvp", matrix.vp_matrix * matrix.model);
        effect->setUniform("col", col);
        effect->setUniform(GL_TEXTURE_2D, "texture", 0, font->getHandle());
        {
            glDisable(GL_CULL_FACE);

            glBindVertexArray(shape->getVAO());
            glDrawElements(GL_TRIANGLES,
                    shape->getIndicesCount(),
                    GL_UNSIGNED_INT,
                    nullptr);
            glBindVertexArray(0);

            glEnable(GL_CULL_FACE);
        }

        effect->end();
    }
}
