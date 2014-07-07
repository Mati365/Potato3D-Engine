#ifndef GAME_HPP_
#define GAME_HPP_
#include "Map.hpp"

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
            unique_ptr<Shape> axis = nullptr, model = nullptr;

            MatrixStack matrix;
            Camera cam = {
                    { 0.f, 1.05f, -1.f, 1.f }, // wysokosc gracza to 1,5m
                    { 0.f, .5f, 1.f, 1.f }
            };

        public:
            GameScreen(Window* _wnd)
                    :
                      wnd(_wnd) {
            }

            inline Window* getWindow() const {
                return wnd;
            }

            void init() override;
            void render() override;
            void getKeyEvent(SDL_Keycode) override;
    };
}

#endif
