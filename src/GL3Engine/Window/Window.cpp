#include "Window.hpp"

namespace GL3Engine {
    bool Window::initialize() noexcept {
        SDL_Init(SDL_INIT_VIDEO);
        window = SDL_CreateWindow(
                "Debug 3D",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                bounds.X,
                bounds.Y,
                SDL_WINDOW_OPENGL
                );

        // SDL_ShowCursor(SDL_FALSE);
        SDL_SetRelativeMouseMode(SDL_TRUE);

        return window != nullptr;
    }
    void Window::initContext() {
        glewExperimental = GL_TRUE;
        glewInit();

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glViewport(0, 0, bounds.X, bounds.Y);
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
                        IPoint2D pos;
                        SDL_GetRelativeMouseState(&pos.X, &pos.Y);
                        renderer->getMouseEvent(
                                pos,
                                event.button.button);
                    }
                        break;

                    case SDL_QUIT:
                        flags = Flags::STOP;
                        break;
                }
            glClearColor(0, 0, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            renderer->render();

            SDL_GL_SwapWindow(window);
        }
        SDL_GL_DeleteContext(gl);
    }

    Window::~Window() {
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}

