#ifndef TEXT_HPP_
#define TEXT_HPP_
#include "Mesh.hpp"

namespace GL3Engine {
    class Font : public Tile {
        public:
            Font(Texture* _tex)
                    :
                      Tile(_tex, IPoint2D(32, 3)) {
            }

            inline TILE_ITER getCharacter(char c) const {
                GLuint index = ((GLuint) c - 32);
                return {
                    uv.begin() + index * 4
                };
            }
    };

    class TextRenderer : public Drawable {
            static constexpr size_t BUFFER_SIZE    =   128;

        private:
            Color col = { 1.f, 1.f, 1.f, 1.f };
            Font* font = nullptr;

            Shape2D* shape = nullptr;
            Mat4 transform = FMAT_MATH::identity();

        public:
            TextRenderer() {
                create();
            }

            void setPos(const FPoint3D&);
            void setSize(GLfloat);
            void setFont(Font* _font) {
                if (_font)
                    font = _font;
            }
            void setText(const string&);

            void draw(MatrixStack&, GLint, Shader*);

            ~TextRenderer() {
                safeDelete(shape, false);
            }
        private:
            void create();
    };
}

#endif
