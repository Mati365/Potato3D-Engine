#ifndef MESH_HPP_
#define MESH_HPP_
#include <map>
#include <functional>
#include <memory>
#include <stdio.h>
#include <fstream>

#include "GL3Engine.hpp"

namespace GL3Engine {
    using namespace std;

    template<typename T> GLint genGLBuffer(const T* data, GLuint len,
            GLint type) {
        GLuint buffer = 0;
        if (data == nullptr)
            return buffer;

        glGenBuffers(1, &buffer);
        glBindBuffer(type, buffer);
        glBufferData(type, len, data, GL_STATIC_DRAW);

        return buffer;
    }

    class Texture {
        private:
            GLuint handle = 0;
            IPoint2D size;

        public:
            Texture() {
            }
            Texture(const string&);

            void loadTexture(const string&);

            GLuint getHandle() const {
                return handle;
            }
            IPoint2D& getSize() {
                return size;
            }

            ~Texture() {
                glDeleteTextures(1, &handle);
            }

        private:
            void configure();
    };
    struct Material {
            string name;
            GLfloat transparent = 0.f,
                    shine = 0.f;
            GLbyte illum_model = 0;

            enum TEX_TYPE
                : GLint {
                    AMBIENT,
                DIFFUSE,
                SPECULAR,
                ALPHA,
                BUMP
            };
            Color col[SPECULAR + 1];
            Texture* tex[BUMP + 1];
    };

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

            Shape(const Vertex* buffer,
                    GLint vertices,
                    const GLushort* i_buffer,
                    GLint indices,
                    const Color& _col)
                    :
                      col(_col) {
                create(buffer, vertices, i_buffer, indices);
            }
            Shape(const Vertex* buffer,
                    GLint vertices,
                    const GLushort* i_buffer,
                    GLint indices) {
                create(buffer, vertices, i_buffer, indices);
            }

            void setColor(const Color& col) {
                this->col = col;
            }
            void draw(MatrixStack&, GLint);

            ~Shape() {
                glDeleteBuffers(1, &vbo);
            }

        private:
            void create(const Vertex*, GLint, const GLushort*, GLint);
    };

    class Mesh : public Drawable {
        public:
            Shape* shape = nullptr;
            FPoint3D pos;
            Mat4 matrix;
    };

    template<typename T>
    class Loader {
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
