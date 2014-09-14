#ifndef LOADERS_HPP_
#define LOADERS_HPP_
#include "Resources.hpp"
#include "../IO.hpp"

namespace GL3Engine {
    using LoaderIterator = std::vector<std::string>::iterator;
    
    /** SHADER */
    namespace CoreLoader {
        using MaterialTextureData = std::array<std::string, CoreMaterial::Material::BUMP + 1>;
        using MaterialData = std::pair<CoreMaterial::Material*, MaterialTextureData>;

        class GLSLloader :
                           public CoreInterface::Loader<CoreEffect::Shader> {
            public:
                static std::string putToFileName(std::string, c_str);
                CoreEffect::Shader* load(c_str);
        };
        class Textureloader :
                              public CoreInterface::Loader<CoreMaterial::Texture> {
            public:
                CoreMaterial::Texture* load(c_str str) {
                    return new CoreMaterial::Texture(str);
                }
        };
    }
    /** MESHE */
    namespace OBJ {
        using HeaderStack = std::vector<CoreMatrix::Vec3>;
        struct IndexStack {
                HeaderStack normals, vertices;
                std::vector<CoreMatrix::Vec2> uv;
        };

        using HeaderMap = std::map<std::string, GLint>;
        using MaterialStack = std::vector<CoreLoader::MaterialData>;

        template<typename T> class ASCIIMeshLoader :
                                                     public CoreInterface::Loader<
                                                             T>,
                                                     public CoreInterface::MemAlloc<
                                                             T> {
            protected:
                HeaderMap headers;

            public:
                ASCIIMeshLoader(const HeaderMap& _headers)
                        :
                          headers(_headers) {
                }

                virtual void onNewHeader(GLint, std::vector<std::string>&) = 0;
                virtual void onHeaderArgument(c_str, GLint,
                        LoaderIterator&) = 0;

                static CoreMatrix::Vec3 getVec3D(LoaderIterator&);
                static CoreMatrix::Vec2 getVec2D(LoaderIterator&);

                T* load(c_str&);
                virtual ~ASCIIMeshLoader() {
                }
                
            private:
                T* selfCreateObject();
        };
        class MTLloader :
                          public ASCIIMeshLoader<CoreMaterial::Materials> {
            private:
                enum HEADER
                                    : GLint {
                        NONE,
                    NAME,
                    SHINE,
                    TRANSPARENT,

                    AMBIENT_COL,
                    DIFFUSE_COL,
                    SPECULAR_COL,

                    AMBIENT_TEX,
                    DIFFUSE_TEX,
                    SPECULAR_TEX,
                    ALPHA_TEX,
                    BUMP_TEX
                };
                MaterialStack materials;

            public:
                MTLloader();

                void onNewHeader(GLint, std::vector<std::string>&) {
                }
                void onHeaderArgument(c_str, GLint, LoaderIterator&);

                /** Dynamiczna alokacja */
                static CoreMaterial::Materials* packTextures(MaterialStack&);
                CoreMaterial::Materials* createObject() override {
                    return MTLloader::packTextures(materials);
                }
                void releaseMemory() override {
                    materials.clear();
                }
        };
        class OBJloader :
                          public ASCIIMeshLoader<SceneObject::Shape3D> {
            private:
                enum HEADER {
                    NONE,
                    VERTEX,
                    NORMAL,
                    TEXTURE,
                    FACE,

                    LOAD_MATERIAL,
                    USE_MATERIAL
                };

                // OBJ
                IndexStack indices;
                CoreType::VertexArray polygon, vertex_array;

                // MTL
                std::unique_ptr<MTLloader> mtl_loader;
                CoreMaterial::Materials materials;
                GLint used_material = -1;

            public:
                OBJloader();

                void onNewHeader(GLint, std::vector<std::string>&);
                void onHeaderArgument(c_str, GLint, LoaderIterator&);

                SceneObject::Shape3D* createObject() override;
                void releaseMemory() override;

            private:
                void finalizePolygon();

                CoreType::Vertex4f getVertex(LoaderIterator& iter);
        };
    }
    namespace MD5 {
        struct Joint {
                std::string name;
                GLint parent_index;
                CoreType::POS pos;
                CoreMatrix::Vec4 orient;
        };
        struct Vertex {
                GLuint index;
                CoreType::UV uv;
                GLuint start_weight;
                GLuint weight_count;
        };
        struct Weight {
                GLuint index;
                GLuint joint_index;
                GLfloat bias;
                CoreType::POS pos;
        };
        struct Pack {
                std::string shader_path;
                std::deque<Vertex> vertices;
                std::deque<CoreMatrix::Vec3> tris;
                std::deque<Weight> weights;
        };
        struct MeshInfo {
                std::deque<Joint> joints;
                std::deque<Pack> meshes;

                inline Pack& lastMesh() {
                    return meshes.back();
                }
        };

        class MD5loader :
                          public CoreInterface::Loader<SceneObject::Shape3D> {
            private:
                std::vector<CoreType::Vertex4f> vertices;
                MeshInfo mesh;

            public:
                SceneObject::Shape3D* load(c_str&);

            private:
                void loadMeshInfo(std::ifstream&);
        };
    }
}

#endif
