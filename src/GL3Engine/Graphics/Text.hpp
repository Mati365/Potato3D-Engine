#ifndef TEXT_HPP_
#define TEXT_HPP_
#include "../Resources/Resources.hpp"
#include "Mesh.hpp"

namespace GL3Engine {
    namespace CoreFont {
        class Font :
                     public CoreMaterial::Tile {
            public:
                Font(CoreMaterial::Texture* _tex)
                        :
                          CoreMaterial::Tile(_tex, CoreMatrix::Vec2i { 32, 4 }) {
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

                static constexpr size_t BUFFER_SIZE = 128;

            private:
                CoreType::Color col = { 1.f, 1.f, 1.f, 1.f };
                std::unique_ptr<Font> font = std::unique_ptr<Font>(
                        new Font(
                                REQUIRE_RES(
                                        CoreMaterial::Texture,
                                        FONT_TEXTURE)));
                SceneObject::Shape2D* shape = nullptr;

            public:
                Text();

                Text& setPos(const CoreMatrix::Vec3&);
                Text& setSize(GLfloat);
                Text& setFont(Font* _font) {
                    if (_font)
                        font.reset(_font);
                    return *this;
                }
                Text& setText(c_str);

                void draw();

            protected:
                void createBuffer();
                void passToShader();
        };
    }
}

#endif
