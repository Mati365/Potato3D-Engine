#ifndef MESH_HPP_
#define MESH_HPP_
#include <map>
#include <functional>
#include <memory>
#include <stdio.h>
#include <fstream>

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
            virtual void draw(MatrixStack&, GLint, Shader*)=0;
            virtual void passToShader(MatrixStack&, Shader*) {
            }

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
            GLboolean hasMaterials() const {
                return !materials.empty();
            }

            Color& getColor() {
                return col;
            }

            GLboolean usingElementBuffer() const {
                return indices_count > 0;
            }

            void changeData(const GL_BUFFER_DATA& vertices,
                    const GL_BUFFER_DATA& indices);

            ~VAOpolygon();

        private:
            void create(const GL_BUFFER_DATA&, const GL_BUFFER_DATA&);
    };

    using Shape3D = VAOpolygon<Vertex4f>;
    using Shape2D = VAOpolygon<Vertex2f>;

    struct Transform {
            Mat4 model = MatMatrix::identity();

            Transform& mul(const Mat4& m) {
                model *= m;
                return *this;
            }
            Transform& identity() {
                MatMatrix::identity(1, &model);
                return *this;
            }
    };
    class Shader;
    class Mesh : public Drawable {
        private:
            Shape3D* shape = nullptr;
            Transform transform;

            vector<MaterialBufferData> material_cache;
            GLuint ubo_handle = 0;

        public:
            Mesh(Shape3D*);
            void draw(MatrixStack&, GLint, Shader*);

            const Shape3D* getShape() const {
                return shape;
            }
            Transform& getTransform() {
                return transform;
            }

        protected:
            void updateMaterialsCache(Shader*);
            void passToShader(MatrixStack&, Shader*);
    };

    /** FBO ma quada i drawable dlatego tutaj */
    class FBO : public Drawable {
        private:
            IPoint2D size;
            GLuint fbo_handle = 0;

            Texture tex;
            unique_ptr<Shape2D> quad;

        public:
            FBO(const IPoint2D&);
            /**
             *  Renderuje bezposrednio na ekran
             *  quada pomijac macierze
             */
            void draw(MatrixStack&, GLint, Shader*);
            /**
             * Przestawienie outputu
             * renderingu, end nie rysuje
             */
            void begin();
            void end();

            GLuint getFBO() const {
                return fbo_handle;
            }
            const IPoint2D& getSize() const {
                return size;
            }
            Texture& getTexture() {
                return tex;
            }

            ~FBO() {
                glDeleteFramebuffers(1, &fbo_handle);
            }
        private:
            void create();
    };
}

#endif
