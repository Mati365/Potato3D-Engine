#ifndef GAME_HPP_
#define GAME_HPP_
#include "Map.hpp"

#include "../GL3Engine/Graphics/Text.hpp"
#include "../GL3Engine/Graphics/Scene.hpp"
#include "../GL3Engine/Graphics/Camera.hpp"
#include "../GL3Engine/Graphics/RenderTarget.hpp"

namespace Game {
    using namespace GL3Engine;
    
    class GameScreen :
                       public Renderer {
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
            RenderQuad* fbo = nullptr;
            GLfloat blur = 0.f;

        public:
            GameScreen(Window*);
            void init() override;
            void draw() override;

            GLboolean getMouseEvent(const Vec2i&, GLuint) override;
            GLboolean getKeyEvent(GLchar) override;

            inline Window* getWindow() const {
                return wnd;
            }
    };
}

#endif
