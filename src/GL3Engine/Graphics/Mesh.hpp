#ifndef MESH_HPP_
#define MESH_HPP_
#include <map>
#include <functional>
#include <memory>
#include <stdio.h>
#include <fstream>

#include "GL3Engine.hpp"
#include "Texture.hpp"
#include "Effects.hpp"

namespace GL3Engine {
    using namespace std;

    struct GL_BUFFER_DATA {
            const GLvoid* data;
            size_t len;
            GLint type;

            GLuint offset;
            GLuint vbo_draw_type;
    };
    GLint genGLBuffer(const GL_BUFFER_DATA&, bool bind = false);

    class Drawable {
        public:
            virtual void draw(MatrixStack&, GLint)=0;
            virtual ~Drawable() {
            }
    };

    template<typename T> class VAOpolygon {
        private:
            GLuint vao = 0,
                    vbo = 0,
                    indices = 0,
                    vertices_count = 0,
                    indices_count = 0;
            Color col;
            MATERIALS materials;

        public:
            VAOpolygon(const GL_BUFFER_DATA& vertices,
                    const GL_BUFFER_DATA& indices,
                    const MATERIALS& _materials)
                    :
                      materials(_materials) {
                create(vertices, indices);
            }
            VAOpolygon(const GL_BUFFER_DATA& vertices,
                    const GL_BUFFER_DATA& indices,
                    const Color& _col)
                    :
                      col(_col) {
                create(vertices, indices);
            }
            VAOpolygon(const GL_BUFFER_DATA& vertices,
                    const GL_BUFFER_DATA& indices) {
                create(vertices, indices);
            }

            GLuint getVAO() const {
                return vao;
            }
            GLuint getVBO() const {
                return vbo;
            }
            GLuint getIndices() const {
                return indices;
            }

            GLuint getVerticesCount() const {
                return vertices_count;
            }
            GLuint getIndicesCount() const {
                return indices_count;
            }
            MATERIALS& getMaterials() {
                return materials;
            }
            Color& getColor() {
                return col;
            }

            GLboolean usingElementBuffer() const {
                return indices_count > 0;
            }

            void changeData(const GL_BUFFER_DATA& vertices,
                    const GL_BUFFER_DATA& indices);

        private:
            void create(const GL_BUFFER_DATA&, const GL_BUFFER_DATA&);
    };

    using Shape3D = VAOpolygon<Vertex4f>;
    using Shape2D = VAOpolygon<Vertex2f>;

    /**
     * Shader odpowiadający za mesha musi mieć
     * określone vertex attributy
     */
    class Shader;
    class Mesh : public Drawable {
        public:
            shared_ptr<Shape3D> shape = nullptr;
            Shader* effect = nullptr; // na 100% w shader managerze

            Mesh(Shape3D*, Shader*);

            void draw(MatrixStack&, GLint);
    };

    template<typename T> class Loader {
        public:
            virtual T* load(ifstream&) = 0;
            virtual ~Loader() {
            }
    };
    class MeshLoader : public Singleton<MeshLoader> {
        private:
            map<string, unique_ptr<Loader<Shape3D>> > loaders;

        public:
            MeshLoader();

            template<typename T> inline void putExtension(
                    const string& extension) {
                loaders[extension] = unique_ptr<T>(new T);
            }
            template<typename T> T* load(const string& path) throw (string) {
                string extension = path.substr(path.find('.') + 1);
                if (!IS_IN_MAP(loaders, "obj"))
                    throw "Unsupported mesh file!";
                ifstream file(path);
                if (!file.is_open())
                    throw "File not found!";
                return loaders[extension]->load(file);
            }
    };
}

#endif
