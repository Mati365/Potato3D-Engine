#ifndef TEXT_HPP_
#define TEXT_HPP_
#include "Mesh.hpp"

namespace GL3Engine {
    class Font : public Tile {
        public:
            Font(const string& path)
                    :
                      Tile(path, IPoint2D(32, 3)) {
            }

            inline GLfloat getRatio() const {
                return cell_size.X / cell_size.Y;
            }
            inline TILE_ITER getCharacter(char c) const {
                GLuint index = ((GLuint) c - 32);
                return {
                    uv.begin() + index * 4
                };
            }
    };

#define BUFFER_SIZE 128
    class TextRenderer : public Drawable, Singleton<TextRenderer> {
        private:
            Color col = { 1.f, 1.f, 1.f, 1.f };
            Font* font = nullptr;

            Shape2D* shape = nullptr;
            Shader* effect = nullptr;
            Mat4 transform = FMAT_MATH::identity();

        public:
            TextRenderer() {
                create();
            }

            void setSize(GLfloat);
            void setFont(Font* _font) {
                font = _font;
            }
            void setText(const string&);
            void draw(MatrixStack&, GLint);

            Shader* getEffect() {
                return effect;
            }

            ~TextRenderer() {
                safeDelete(shape, false);
            }
        private:
            void create();
    };
}

#endif
