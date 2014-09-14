#include "Text.hpp"

#include "../IO.hpp"
#include "../Resources/Resources.hpp"

namespace GL3Engine {
    namespace CoreFont {
        TYPE_IMPORT(CoreMatrix, Vec3);
        TYPE_IMPORT(CoreMatrix, Vec2);
        TYPE_IMPORT(CoreMatrix, MatMatrix);
        TYPE_IMPORT(CoreType, Vertex2f);
        TYPE_IMPORT(CoreMaterial, TileIterator);

        TYPE_IMPORT(std, vector);
        TYPE_IMPORT(std, string);
        
        Text::Text() {
            setEffect(REQUIRE_RES(CoreEffect::Shader, DEFAULT_TEXT_SHADER));
            createBuffer();
        }
        
        Text& Text::setPos(const Vec3& pos) {
            MatMatrix::translate(this->transform.model, pos);
            return *this;
        }
        Text& Text::setSize(GLfloat size) {
            MatMatrix::scale(this->transform.model,
                    { size, size / font->getCells().Y(), 1.f });
            return *this;
        }
        Text& Text::setText(const string& text) {
            if (!font)
                return *this;

            vector<Vertex2f> vertex_buffer;
            vector<GLuint> index_buffer;

            Vec2 cursor = { 0.f, 0.f };
            Vec2 cell_size = font->getCellSize();

            for (char c : text) {
                if (c == '\n') {
                    cursor[0] = 0.f;
                    cursor[1] -= cell_size[1];
                    continue;
                }
                TileIterator iter = font->getCharacter(c);
                {
                    // UV
                    for (GLuint i = 0; i < 4; ++i) {
                        TileIterator _v = iter + i;
                        vertex_buffer.push_back(
                                {
                                        { _v->pos[0] + cursor[0], _v->pos[1]
                                                + cursor[1], _v->pos[2] },
                                        { _v->uv[0], _v->uv[1] }
                                });
                    }
                    // Index
                    for (GLuint i = 0; i < 6; ++i)
                        for (GLfloat obj : CoreMaterial::Tile::quad_indices)
                            index_buffer.push_back(
                                    obj + vertex_buffer.size() - 4);
                }
                cursor[0] += cell_size[0];
            }
            shape->changeData( {
                    &vertex_buffer[0], vertex_buffer.size() * sizeof(Vertex2f),
                    GL_ARRAY_BUFFER, 0,
                    GL_DYNAMIC_DRAW }, {
                    &index_buffer[0], index_buffer.size() * sizeof(GLuint),
                    GL_ELEMENT_ARRAY_BUFFER, 0,
                    GL_DYNAMIC_DRAW });
            return *this;
        }
        void Text::createBuffer() {
            shape =
                    new SceneObject::Shape2D(
                            { nullptr, BUFFER_SIZE * sizeof(Vertex2f), GL_ARRAY_BUFFER, 0, GL_DYNAMIC_DRAW },
                            { nullptr, BUFFER_SIZE * sizeof(GLuint), GL_ELEMENT_ARRAY_BUFFER, 0, GL_DYNAMIC_DRAW },
                            col);
        }
        
        void Text::passToShader() {
            assert(effect);
            {
                effect->setUniform("col", col)
                        .setUniform(GL_TEXTURE_2D, "texture", 0,
                        font->getHandle())
                        .setUniform("matrix.mvp",
                        world->attrib.vp_matrix * world->attrib.model
                                * transform.model);
            }
        }
        void Text::draw() {
            assert(font && shape && effect);
            {
                passToShader();
                glDisable(GL_CULL_FACE);
                {
                    glBindVertexArray(shape->getVAO());
                    glDrawElements(GL_TRIANGLES, shape->getIndicesCount(),
                    GL_UNSIGNED_INT, nullptr);
                    glBindVertexArray(0);
                }
                glEnable(GL_CULL_FACE);
            }
        }
    }
}
