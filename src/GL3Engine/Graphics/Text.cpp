#include "Text.hpp"

#include "../IO.hpp"
#include "../Resources/Resources.hpp"

namespace GL3Engine {
    using namespace IO;

    Text::Text() {
        setEffect(REQUIRE_RES(Shader, DEFAULT_TEXT_SHADER));
        createBuffer();
    }

    Text& Text::setPos(const FPoint3D& pos) {
        MatMatrix::translate(this->transform.model, pos);
        return *this;
    }
    Text& Text::setSize(GLfloat size) {
        MatMatrix::scale(
                this->transform.model, {
                        size,
                        size / font->getCells().Y,
                        1.f });
        return *this;
    }
    Text& Text::setText(const string& text) {
        if (!font)
            return *this;

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
                            Tile::quad_indices[i] + vertex_buffer.size() - 4);
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
        return *this;
    }
    void Text::createBuffer() {
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
    }

    void Text::passToShader() {
        MatrixStack& matrix = scene->getWorldMatrix();
        assert(effect);
        {
            effect->setUniform("col", col)
                    .setUniform(GL_TEXTURE_2D, "texture", 0, font->getHandle())
                    .setUniform("matrix.mvp",
                    matrix.vp_matrix * matrix.model * transform.model);
        }
    }
    void Text::draw() {
        assert(font && shape && effect);
        {
            passToShader();
            glDisable(GL_CULL_FACE);
            {
                glBindVertexArray(shape->getVAO());
                glDrawElements(
                GL_TRIANGLES,
                        shape->getIndicesCount(),
                        GL_UNSIGNED_INT,
                        nullptr);
                glBindVertexArray(0);
            }
            glEnable(GL_CULL_FACE);
        }
    }
}
