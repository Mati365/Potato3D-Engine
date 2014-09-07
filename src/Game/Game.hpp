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
                       public CoreWindow::Renderer {
        public:
            enum Flags {
                DRAW_AXIS = 1 << 1,
                DRAW_RENDERER_INFO = 1 << 2
            };
            GLuint flags = 0;

        private:
            CoreWindow::Window* wnd = nullptr;
            CoreRenderer::SceneManager scene;
            SceneObject::RenderQuad* fbo = nullptr;
            SceneObject::Mesh* mesh = nullptr;

            GLfloat blur = 0.f;

        public:
            GameScreen(CoreWindow::Window*);
            void init() override;
            void draw() override;

            GLboolean getMouseEvent(const CoreMatrix::Vec2i&, GLuint) override;
            GLboolean getKeyEvent(GLchar) override;

            inline CoreWindow::Window* getWindow() const {
                return wnd;
            }
    };
}

#endif
