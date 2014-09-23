#ifndef TEXT_HPP_
#define TEXT_HPP_
#include "../Resources/Resources.hpp"
#include "Mesh.hpp"

namespace GL3Engine {
    namespace CoreFont {
        class Font :
                     public CoreMaterial::Tile {
            public:
                Font(CoreMaterial::Texture* tex)
                        :
                          CoreMaterial::Tile(tex, CoreMatrix::Vec2i { 32, 4 }) {
                }
                CoreMaterial::TileIterator getCharacter(GLchar c) const {
                    GLuint index = ((GLuint) c - 32);
                    return {
                        uv.begin() + index * 4
                    };
                }
        };
        class Text :
                     public CoreRenderer::Node {
            DECLARE_NODE_TYPE(Text)

                static constexpr size_t BUFFER_SIZE = 256 * 4
                        * sizeof(CoreType::Vertex2f);

            private:
                CoreType::Color col = { 1.f, 1.f, 1.f, 1.f };
                Font font = Font(
                        REQUIRE_RES(CoreMaterial::Texture, FONT_TEXTURE));
                std::unique_ptr<SceneObject::Shape2D> shape =
                        std::unique_ptr<SceneObject::Shape2D>(
                                new SceneObject::Shape2D(
                                        {
                                                nullptr,
                                                BUFFER_SIZE
                                                        * sizeof(CoreType::Vertex2f),
                                                GL_ARRAY_BUFFER, 0,
                                                GL_DYNAMIC_DRAW
                                        },
                                        {
                                                nullptr,
                                                BUFFER_SIZE * sizeof(GLuint),
                                                GL_ELEMENT_ARRAY_BUFFER, 0,
                                                GL_DYNAMIC_DRAW
                                        },
                                        col));

            public:
                Text();
                void draw() override;

                Text& setPos(const CoreMatrix::Vec3&);
                Text& setSize(GLfloat);
                Text& setFont(const Font& font) {
                    this->font = font;
                    return *this;
                }
                Text& setText(c_str);

            protected:
                void createBuffer();
                void passToShader() override;
        };
    }
}

#endif
