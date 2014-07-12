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
            unique_ptr<Mesh> axis = nullptr, model = nullptr;

            MatrixStack matrix;
            Camera cam = {
                    { 0.f, 1.05f, 1.f, 1.f }, // wysokosc gracza to 1,5m
                    { 0.f, 0.05f, -1.f, 1.f }
            };

            Font* font = nullptr;
            TextRenderer* text = nullptr;

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
