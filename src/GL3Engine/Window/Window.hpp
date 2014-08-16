#ifndef WINDOW_HPP_
#define WINDOW_HPP_
#include <SDL2/SDL.h>

#include "../GL3Engine.hpp"
#include "../Graphics/Primitives.hpp"

namespace GL3Engine {
    class Renderer :
                     public Drawable,
                     public WindowEventListener {
        public:
            virtual void init() = 0;
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
            Vec2i bounds;
            Renderer* renderer = nullptr;

        public:
            Window(const Vec2i& _bounds)
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
            
            const Vec2i& getBounds() const {
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
