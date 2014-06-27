#include <iostream>

#include "G3Engine/Graphics/Graphics.hpp"

using namespace std;

namespace Engine {
    using namespace Graphics;
    /** Rendering prymitywów */
    class Primitives {
        public:
            static Shape* genAxis(GLfloat size) {
                if (!size % 2)
                    return nullptr;

                vector<Vertex> sheet;
                GLfloat start_pos = floor(sqrt(size));
                for (GLint i = 0; i < (GLint) size; ++i) {
                    sheet.push_back( {
                            { i * .5f - start_pos, 0.f, -start_pos },
                            { 1.f, 1.f, 1.f },
                            { 0.f, 0.f }
                    });
                    sheet.push_back( {
                            { i * .5f - start_pos, 0.f, start_pos },
                            { 1.f, 1.f, 1.f },
                            { 0.f, 0.f }
                    });
                    sheet.push_back( {
                            { start_pos, 0.f, -start_pos + i * .5f },
                            { 1.f, 1.f, 1.f },
                            { 0.f, 0.f }
                    });
                    sheet.push_back( {
                            { -start_pos, 0.f, -start_pos + i * .5f },
                            { 1.f, 1.f, 1.f },
                            { 0.f, 0.f }
                    });
                }
                return new Shape(
                        &sheet[0],
                        sheet.size(),
                        nullptr,
                        0,
                        Color(.25f, .25f, .25f, 1.f));
            }
    };

    /**
     * Zastosowanie kompozycji,
     * dziedziczenie odpada bo
     * w trakcie życia meshu
     * będzie można zmieniać mu
     * siatki
     */
    Shape* loadOBJ(const char* path) {
        enum Header {
            VERTEX = 1,
            NORMAL,
            UV,
            FACE
        };
        map<string, Header> headers;
        headers["v"] = VERTEX;
        headers["vn"] = NORMAL;
        headers["vt"] = UV;
        headers["f"] = FACE;

        FILE* fp = fopen(path, "r");

        vector<FPoint3D> vertices, normals;
        vector<FPoint2D> uv;

        vector<Vertex> vertex;
        while (!feof(fp)) {
            char header[2];
            fscanf(fp, "%3s", header);

            Header type = headers[string(header)];
            switch (type) {
                case NORMAL:
                    case VERTEX: {
                    FPoint3D v;
                    fscanf(fp, "%f %f %f\n", &v.X, &v.Y, &v.Z);
                    vertices.push_back(v);
                }
                    break;

                case FACE: {
                    GLuint v[3];
                    fscanf(fp, "%d %d %d\n", &v[0], &v[1], &v[2]);
                    for (GLint i = 0; i < 3; ++i)
                        vertex.push_back(
                                {
                                        {
                                                vertices[v[i] - 1].X,
                                                vertices[v[i] - 1].Y,
                                                vertices[v[i] - 1].Z
                                        },
                                        { 1.f, 1.f, 1.f },
                                        { 1.f, 1.f }
                                });
                }
                    break;

                default:
                    break;
            }
        }
        return new Shape(&vertex[0],
                vertex.size(),
                nullptr,
                0,
                { 1.f, 0.f, 0.f, 1.f });
    }
    class Mesh : public Drawable {
        public:
            Shape* shape = nullptr;
            FPoint3D pos;
            Mat4 rotation;
    };

    class Renderer {
        public:
            virtual void init() = 0;
            virtual void render() = 0;
            virtual ~Renderer() {
            }
    };
    class GL3Renderer : public Renderer {
        private:
            enum Flags {
                DRAW_AXIS = 1 << 1
            };
            GLuint flags = 0;

            MatrixStack matrix;
            Camera cam;

            Shape *axis = nullptr,
                    *model = nullptr;

        public:
            void init() {
                axis = Primitives::genAxis(17);

                cam.pos[1] += 1.f;
                cam.pos[0] += 2.f;
                matrix.setCameraCoords(cam);

                model = loadOBJ("model.obj");
            }
            void render() {
                if (axis != nullptr)
                    axis->draw(matrix, GL_LINES);
                if (model != nullptr) {
                    matrix.pushTransform();
                    matrix.model *= FMAT_MATH::scale( { 17.f, 17.f, 17.f });

                    model->draw(matrix, GL_LINES);
                    matrix.popTransform();
                }
            }
    };
}
namespace Window {
    using namespace Tools;
    using namespace Engine;

    class Window {
        public:
            enum Flags {
                STOP = 0,
                RUNNING = 1 << 0
            };

        private:
            GLuint flags = Flags::RUNNING;

            SDL_Window* window;
            IPoint2D bounds;
            Renderer* renderer = nullptr;

        public:
            Window(const IPoint2D& _bounds)
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

            ~Window() {
                SDL_DestroyWindow(window);
                SDL_Quit();
            }

        protected:
            bool initialize() noexcept {
                SDL_Init(SDL_INIT_VIDEO);
                window = SDL_CreateWindow(
                        "Debug",
                        SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED,
                        bounds.X,
                        bounds.Y,
                        SDL_WINDOW_OPENGL
                        );

                return window != nullptr;
            }
            void initContext() {
                glewExperimental = GL_TRUE;
                glewInit();

                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

                SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
                SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LESS);
            }
            void run() {
                SDL_GLContext gl = SDL_GL_CreateContext(window);
                SDL_Event event;

                initContext();
                renderer->init();

                while (IS_SET_FLAG(flags, Flags::RUNNING)) {
                    while (SDL_PollEvent(&event))
                        switch (event.type) {
                            case SDL_KEYDOWN:
                                switch (event.key.keysym.sym) {
                                    case SDLK_LEFT:
                                        break;

                                    case SDLK_RIGHT:
                                        break;

                                    case SDLK_UP:
                                        break;

                                    case SDLK_DOWN:
                                        break;
                                }
                                break;

                            case SDL_QUIT:
                                flags = Flags::STOP;
                                break;
                        }
                    glClearColor(0, 0, 0, 1);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    if (renderer != nullptr)
                        renderer->render();

                    SDL_GL_SwapWindow(window);
                }
                SDL_GL_DeleteContext(gl);
            }
    };
}
int main() {
    try {
        Window::Window wnd( { 400, 400 });
        boost::scoped_ptr<Engine::GL3Renderer> renderer(
                new Engine::GL3Renderer());
        wnd.setRenderer(renderer.get());
        wnd.open();
    } catch (const string& ex) {
        cout << ex;
    }
    return 0;
}
