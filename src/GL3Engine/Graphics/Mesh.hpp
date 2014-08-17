#ifndef MESH_HPP_
#define MESH_HPP_
#include <map>
#include <functional>
#include <memory>
#include <stdio.h>
#include <fstream>

#include "Scene.hpp"

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
    
    template<typename T> class VAOpolygon {
        private:
            GLuint vao = 0, vbo = 0, indices = 0, vertices_count = 0,
                    indices_count = 0;
            Color col;
            MATERIALS materials;

        public:
            VAOpolygon(
                    const GL_BUFFER_DATA& vertices,
                    const GL_BUFFER_DATA& indices, const Color& _col = { },
                    const MATERIALS& _materials = { })
                    :
                      col(_col),
                      materials(_materials) {
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
            
            void changeData(
                    const GL_BUFFER_DATA& vertices,
                    const GL_BUFFER_DATA& indices);

            virtual ~VAOpolygon();

        private:
            void create(const GL_BUFFER_DATA&, const GL_BUFFER_DATA&);
    };
    extern template class VAOpolygon<Vertex4f> ;
    extern template class VAOpolygon<Vertex2f> ;
    
    using Shape3D = VAOpolygon<Vertex4f>;
    using Shape2D = VAOpolygon<Vertex2f>;
    
    class Mesh :
                 public Node {
        DECLARE_NODE_TYPE(Mesh)

        private:
            Shape3D* shape = nullptr;

            vector<MaterialBufferData> material_cache;
            GLuint ubo_handle = 0;

        public:
            Mesh(Shape3D* _shape = nullptr);
            void draw() override;

            Mesh& setShape(Shape3D* shape) {
                this->shape = shape;
                material_cache.clear();
                return *this;
            }
            const Shape3D* getShape() const {
                return shape;
            }
            
        protected:
            void updateMaterialsCache(Shader*);
            void passToShader();
    };
}

#endif
