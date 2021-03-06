#include "Window.hpp"

namespace GL3Engine {
    namespace CoreWindow {
        TYPE_IMPORT(CoreMatrix, Vec2i);

        bool Window::initialize() noexcept {
            SDL_Init(SDL_INIT_VIDEO);
            window = SDL_CreateWindow(
                    "Debug 3D",
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    bounds.X(), bounds.Y(),
                    SDL_WINDOW_OPENGL);
            SDL_SetRelativeMouseMode(SDL_TRUE);
            return window != nullptr;
        }
        void Window::initContext() {
            glewExperimental = GL_TRUE;
            glewInit();

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glAlphaFunc(GL_GREATER, .5f);
            glEnable(GL_ALPHA_TEST);

            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);

            glClearColor(0.f, 0.f, 0.f, 1.f);
        }
        void Window::run() {
            if (!renderer)
                return;
            SDL_GLContext gl = SDL_GL_CreateContext(window);
            SDL_Event event;
            
            initContext();
            renderer->init();
            
            while (IS_SET_FLAG(flags, Flags::RUNNING)) {
                while (SDL_PollEvent(&event))
                    switch (event.type) {
                        case SDL_KEYDOWN:
                            renderer->getKeyEvent(event.key.keysym.sym);
                            break;

                        case SDL_MOUSEBUTTONDOWN:
                            case SDL_MOUSEMOTION: {
                            Vec2i pos;
                            SDL_GetRelativeMouseState(
                                    &pos[0],
                                    &pos[1]);
                            renderer->getMouseEvent(pos, event.button.button);
                        }
                            break;

                        case SDL_QUIT:
                            flags = Flags::STOP;
                            break;
                    }
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                renderer->draw();
                SDL_GL_SwapWindow(window);
            }
            SDL_GL_DeleteContext(gl);
        }

        Window::~Window() {
            SDL_DestroyWindow(window);
            SDL_Quit();
        }
    }
}

