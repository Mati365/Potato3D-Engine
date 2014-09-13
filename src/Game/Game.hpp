#ifndef GAME_HPP_
#define GAME_HPP_
#include "Map.hpp"

#include "../GL3Engine/Graphics/Text.hpp"
#include "../GL3Engine/Graphics/Scene.hpp"
#include "../GL3Engine/Graphics/Camera.hpp"
#include "../GL3Engine/Graphics/RenderTarget.hpp"

namespace Game {
    class GameScreen :
                       public GL3Engine::CoreWindow::Renderer {
        public:
            enum Flags {
                DRAW_AXIS = 1 << 1,
                DRAW_RENDERER_INFO = 1 << 2
            };
            GLuint flags = 0;

        private:
            GL3Engine::CoreWindow::Window* wnd = nullptr;
            GL3Engine::CoreRenderer::SceneManager scene;
            GL3Engine::SceneObject::RenderQuad* fbo = nullptr;
            GL3Engine::SceneObject::Mesh* mesh = nullptr;

            GLfloat blur = 0.f;

        public:
            GameScreen(GL3Engine::CoreWindow::Window*);
            void init() override;
            void draw() override;

            GLboolean getMouseEvent(const GL3Engine::CoreMatrix::Vec2i&, GLuint)
                    override;
            GLboolean getKeyEvent(GLchar) override;

            inline GL3Engine::CoreWindow::Window* getWindow() const {
                return wnd;
            }
    };
}

#endif
