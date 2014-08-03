#ifndef TEXT_HPP_
#define TEXT_HPP_
#include "../Resources/Resources.hpp"
#include "Mesh.hpp"

namespace GL3Engine {
    class Font : public Tile {
        public:
            Font(Texture* _tex)
                    :
                      Tile(_tex, IPoint2D(32, 4)) {
            }

            inline TILE_ITER getCharacter(char c) const {
                GLuint index = ((GLuint) c - 32);
                return {
                    uv.begin() + index * 4
                };
            }
    };
    class Text : public Node {
            static constexpr size_t BUFFER_SIZE = 128;

        private:
            Color col = { 1.f, 1.f, 1.f, 1.f };
            unique_ptr<Font> font = unique_ptr < Font > (
                    new Font(
                            REQUIRE_RES(Texture, FONT_TEXTURE)));

            Shape2D* shape = nullptr;

        public:
            Text();

            Text& setPos(const FPoint3D&);
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

#endif
