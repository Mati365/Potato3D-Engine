#ifndef GAME_HPP_
#define GAME_HPP_
#include "Map.hpp"

#include "../GL3Engine/Graphics/Text.hpp"

namespace Game {
    using namespace GL3Engine;

    class GameScreen : public Renderer {
        public:
            enum Flags {
                DRAW_AXIS = 1 << 1
            };
            GLuint flags = 0;

        private:
            Window* wnd;
            unique_ptr<Mesh> axis = nullptr;
            Mesh* model = nullptr, *box = nullptr;

            MatrixStack matrix;
            Camera cam = {
                    Vec4 { 0.f, .5f, 1.f, 1.f }, // wysokosc gracza to 1,5m
                    Vec4 { 0.f, .5f, -2.f, 1.f }
            };

        public:
            GameScreen(Window* _wnd)
                    :
                      wnd(_wnd),
                      matrix(_wnd->getBounds()){
            }

            inline Window* getWindow() const {
                return wnd;
            }

            void init() override;
            void render() override;

            void getMouseEvent(const IPoint2D&, GLuint) override;
            void getKeyEvent(SDL_Keycode) override;
    };
}

#endif
