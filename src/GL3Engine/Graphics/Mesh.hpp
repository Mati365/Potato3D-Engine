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

#define MINIMUM_VRAM_SIZE   (64*1024*1024) // wielkość VBOBath

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

    struct VAO_BATH_PTR {
            GLsizeiptr vbo_offset,
                    indices_offset,
                    vbo_length,
                    indices_length;
    };
    class VBOBath : public Singleton<VBOBath> {
        private:
            GLuint vao = 0,
                    vbo = 0,
                    indices = 0;
            GLsizeiptr last_vbo_offset = 0,
                    last_index_offset = 0;

        public:
            VBOBath() {
                VBOBath(MINIMUM_VRAM_SIZE);
            }
            VBOBath(GLsizeiptr);

            VAO_BATH_PTR putBuffers(const GL_BUFFER_DATA&,
                    const GL_BUFFER_DATA&);

#define HANDLE_GETTER(name, variable) inline GLuint get##name##Buffer() const { return variable ; }

            HANDLE_GETTER(VBO, vbo)
            HANDLE_GETTER(VAO, vao)
            HANDLE_GETTER(Indices, indices)

        private:
            static void assignBuffer(GLsizeiptr*,
                    GLsizeiptr*, const GL_BUFFER_DATA&,
                    GLint, GLsizeiptr*);
    };

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
    class TextureArray {
        private:
            GLuint handle = 0;
            vector<string> textures;

        public:
            TextureArray() {
            }
            TextureArray(const vector<string>&);

            void create();

            inline void addTexture(c_str str) {
                textures.push_back(str);
            }
            GLuint getHandle() const {
                return handle;
            }
    };

    struct Material {
            enum TEX_TYPE
                : GLint {
                    AMBIENT,
                DIFFUSE,
                SPECULAR,
                ALPHA,
                BUMP
            };

            GLfloat transparent = 1.f,
                    shine = 1.f;
            GLbyte illum_model = 0;
            Color col[SPECULAR + 1];

            string tex[BUMP + 1];
            string name;
            GLuint tex_array_handle = 0; // handle wspóldzielony ze wszystkimi materialami w meshu

            Material() {
            }
            Material(const Material& mat) {
                name = mat.name;
                transparent = mat.transparent;
                shine = mat.shine;
                illum_model = mat.illum_model;

                memcpy(col, mat.col, sizeof(col));
                memcpy(tex, mat.tex, sizeof(tex));
            }
            Material(const Color& diffuse_color) {
                col[DIFFUSE] = diffuse_color;
            }
    };

    using MATERIALS = vector<Material*>;

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

            Shape(const Vertex* buffer,
                    GLint vertices,
                    const GLushort* i_buffer,
                    GLint indices,
                    const MATERIALS& _materials)
                    :
                      materials(_materials) {
                create(buffer, vertices, i_buffer, indices);
            }
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
