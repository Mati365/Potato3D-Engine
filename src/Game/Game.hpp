#ifndef GAME_HPP_
#define GAME_HPP_
#include "Map.hpp"

#include "../GL3Engine/Graphics/Text.hpp"
#include "../GL3Engine/Graphics/Scene.hpp"
#include "../GL3Engine/Graphics/Camera.hpp"

namespace Game {
    using namespace GL3Engine;

    class GameScreen : public Renderer {
        public:
            enum Flags {
                DRAW_AXIS = 1 << 1,
                DRAW_RENDERER_INFO = 1 << 2
            };
            GLuint flags = 0;

        private:
            Window* wnd;

            SceneManager scene;
            Camera* cam = nullptr;

        public:
            GameScreen(Window*);

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
