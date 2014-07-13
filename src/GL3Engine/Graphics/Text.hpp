#ifndef TEXT_HPP_
#define TEXT_HPP_
#include "Mesh.hpp"

namespace GL3Engine {

#define FONT_SHEET_SIZE     IPoint2D(32, 3)
#define START_CHARACTER     32
    class Font : public Tile {
        public:
            Font(const string& path)
                    :
                      Tile(path, FONT_SHEET_SIZE) {
            }

            inline TILE_ITER getCharacter(char c) const {
                GLuint index = ((GLuint) c - START_CHARACTER);
                return {
                    uv.begin() + index * 4
                };
            }
    };

#define BUFFER_SIZE 256
    class TextRenderer : public Drawable, Singleton<TextRenderer> {
        private:
            Color col = { 1.f, 1.f, 1.f, 1.f };
            Font* font = nullptr;

            Shape2D* shape = nullptr;
            Shader* effect = nullptr;

        public:
            TextRenderer() {
                create();
            }

            void setFont(Font* _font) {
                font = _font;
            }
            void setText(const string&);
            void draw(MatrixStack&, GLint);

            ~TextRenderer() {
                safeDelete(shape, false);
            }
        private:
            void create();
    };
}

#endif
