#ifndef WINDOW_HPP_
#define WINDOW_HPP_
#include <SDL2/SDL.h>

#include "../Graphics/GL3Engine.hpp"
#include "../Graphics/Primitives.hpp"

namespace GL3Engine {
    class Renderer {
        public:
            virtual void init() = 0;
            virtual void render() = 0;
            virtual void getKeyEvent(SDL_Keycode) = 0;

            virtual ~Renderer() {
            }
    };
    class Window {
        public:
            enum Flags {
                STOP = 0,
                RUNNING = 1 << 0
            };

        private:
            GLuint flags = Flags::RUNNING;

            SDL_Window* window;
            FPoint2D bounds;
            Renderer* renderer = nullptr;

        public:
            Window(const FPoint2D& _bounds)
                    :
                      bounds(_bounds) {
                if (!initialize())
                    throw "Nie mogłem otworzyć okna!";
            }

            void open() {
                run();
            }
            void setRenderer(Renderer* renderer) {
                this->renderer = renderer;
            }
            Renderer* getRenderer() {
                return renderer;
            }

            const FPoint2D& getBounds() const {
                return bounds;
            }

            ~Window();

        protected:
            bool initialize() noexcept;
            void initContext();
            void run();
    };
}

#endif
