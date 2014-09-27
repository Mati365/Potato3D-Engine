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
        }
        
        Text& Text::setPos(const Vec3& pos) {
            MatMatrix::translate(this->transform.model, pos);
            return *this;
        }
        Text& Text::setSize(GLfloat size) {
            MatMatrix::scale(this->transform.model,
                    { size, size / font.getCells().Y(), 1.f });
            return *this;
        }
        Text& Text::setText(c_str text) {
            vector<Vertex2f> vertex_buffer;
            vector<GLuint> index_buffer;

            Vec2 cursor = { 0.f, 0.f };
            Vec2 cell_size = font.getCellSize();

            for (GLchar c : text) {
                if (c == '$') {
                    cursor[0] = 0.f;
                    cursor[1] -= cell_size[1];
                    continue;
                }
                // Vertex/UV
                TileIterator iter = font.getCharacter(c);
                for (TileIterator _v = iter; _v < iter + 4; _v++)
                    vertex_buffer.push_back(
                            {
                                    {
                                            _v->pos[0] + cursor[0],
                                            _v->pos[1] + cursor[1],
                                            _v->pos[2]
                                    },
                                    {
                                            _v->uv[0],
                                            _v->uv[1]
                                    }
                            });
                // Index
                for (GLfloat obj : CoreMaterial::Tile::quad_indices)
                    index_buffer.push_back(obj + vertex_buffer.size() - 4);
                cursor[0] += cell_size[0];
            }
            if (vertex_buffer.size() * sizeof(Vertex2f) > Text::BUFFER_SIZE) {
                LOG(ERROR, "Not enough memory!");
                return *this;
            }
            shape->changeData(
                    {
                            &vertex_buffer[0],
                            vertex_buffer.size() * sizeof(Vertex2f),
                            GL_ARRAY_BUFFER, 0,
                            GL_DYNAMIC_DRAW
                    },
                    {
                            &index_buffer[0],
                            index_buffer.size() * sizeof(GLuint),
                            GL_ELEMENT_ARRAY_BUFFER, 0,
                            GL_DYNAMIC_DRAW
                    });
            return *this;
        }
        
        void Text::passToShader() {
            assert(effect);
            {
                effect->setUniform("col", col)

                .setUniform(GL_TEXTURE_2D, "texture", 0,
                        font.getHandle())

                .setUniform("matrix.mvp",
                        world->attrib.vp_matrix *
                                world->attrib.model *
                                transform.model);
            }
        }
        void Text::draw() {
            assert(shape && effect);
            {
                passToShader();
                glDisable(GL_CULL_FACE);
                {
                    glBindVertexArray(shape->getVAO());
                    glDrawElements(GL_TRIANGLES,
                            shape->getIndicesCount(),
                            GL_UNSIGNED_INT, nullptr);
                    glBindVertexArray(0);
                }
                glEnable(GL_CULL_FACE);
            }
        }
    }
}
