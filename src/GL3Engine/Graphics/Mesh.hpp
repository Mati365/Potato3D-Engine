#ifndef MESH_HPP_
#define MESH_HPP_
#include <map>
#include <functional>
#include <memory>
#include <stdio.h>
#include <fstream>

#include "Scene.hpp"

namespace GL3Engine {
    namespace SceneObject {
        template<typename T>
        class VAOpolygon :
                           public CoreInterface::NonCopyable {
            private:
                GLuint vao = 0, vbo = 0, indices = 0, vertices_count = 0,
                        indices_count = 0;
                CoreType::Color col;
                CoreMaterial::Materials materials;

            public:
                VAOpolygon(
                        const CoreType::BufferData&,
                        const CoreType::BufferData&,
                        const CoreType::Color& _col = { },
                        const CoreMaterial::Materials& _materials = { });

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

                CoreMaterial::Materials& getMaterials() {
                    return materials;
                }
                GLboolean hasMaterials() const {
                    return !materials.empty();
                }

                CoreType::Color& getColor() {
                    return col;
                }
                GLboolean usingElementBuffer() const {
                    return indices_count > 0;
                }

                void changeData(
                        const CoreType::BufferData&,
                        const CoreType::BufferData&);

                virtual ~VAOpolygon();

            private:
                void create(
                        const CoreType::BufferData&,
                        const CoreType::BufferData&);
        };
        extern template class VAOpolygon<CoreType::Vertex4f> ;
        extern template class VAOpolygon<CoreType::Vertex2f> ;

        using Shape3D = VAOpolygon<CoreType::Vertex4f>;
        using Shape2D = VAOpolygon<CoreType::Vertex2f>;

        class Mesh :
                     public CoreRenderer::Node {
            DECLARE_NODE_TYPE(Mesh)

            public:
                enum Flags {
                    NONE = 0,
                    USE_MATERIALS = 1 << 1,
                    USE_LIGHTING = 1 << 2
                };

            private:
                Shape3D* shape = nullptr;
                std::vector<CoreMaterial::MaterialBufferData> material_cache;

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
                void updateMaterialsCache(CoreEffect::Shader*);
                void passToShader();
        };
    }
}

#endif
