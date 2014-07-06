#ifndef MESH_HPP_
#define MESH_HPP_
#include <map>
#include <functional>
#include <memory>
#include <stdio.h>
#include <fstream>

#include "GL3Engine.hpp"
#include "Texture.hpp"

namespace GL3Engine {
    using namespace std;

    struct GL_BUFFER_DATA {
            const GLvoid* data;
            GLsizeiptr len;
            GLint type;

            GL_BUFFER_DATA()
                    :
                      data(nullptr),
                      len(0),
                      type(0) {
            }
            GL_BUFFER_DATA(const GLvoid* _data, GLsizeiptr _len, GLint _type)
                    :
                      data(_data),
                      len(_len),
                      type(_type) {
            }
    };
    GLint genGLBuffer(const GL_BUFFER_DATA&, bool bind = false);

    class Drawable {
        public:
            virtual void draw(MatrixStack&, GLint)=0;
            virtual ~Drawable() {
            }
    };
    class Shape : public Drawable {
        public:
            GLuint vao = 0,
                    vbo = 0,
                    indices = 0,
                    vertices_count = 0,
                    indices_count = 0;
            Color col;
            MATERIALS materials;

            Shape(const GL_BUFFER_DATA& vertices,
                    const GL_BUFFER_DATA& indices,
                    const MATERIALS& _materials)
                    :
                      materials(_materials) {
                create(vertices, indices);
            }
            Shape(const GL_BUFFER_DATA& vertices,
                    const GL_BUFFER_DATA& indices,
                    const Color& _col)
                    :
                      col(_col) {
                create(vertices, indices);
            }
            Shape(const GL_BUFFER_DATA& vertices,
                    const GL_BUFFER_DATA& indices) {
                create(vertices, indices);
            }

            void draw(MatrixStack&, GLint);

        private:
            void create(const GL_BUFFER_DATA&, const GL_BUFFER_DATA&);
    };
    class Mesh : public Drawable {
        public:
            Shape* shape = nullptr;
            FPoint3D pos;
            Mat4 matrix;
    };

    template<typename T> class Loader {
        public:
            virtual T* load(ifstream&) = 0;
            virtual ~Loader() {
            }
    };
    class MeshLoader : public Singleton<MeshLoader> {
        private:
            map<string, unique_ptr<Loader<Shape>> > loaders;

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
