#ifndef LOADERS_HPP_
#define LOADERS_HPP_
#include "Resources.hpp"
#include "../IO.hpp"

namespace GL3Engine {
    using namespace IO;
    using LOADER_ITERATOR = vector<string>::iterator;
    
    /** SHADER */
    class GLSLloader :
                       public Loader<Shader> {
        public:
            static string putToFileName(string, c_str);
            Shader* load(c_str);
    };
    class Textureloader :
                          public Loader<Texture> {
        public:
            Texture* load(c_str str) {
                return new Texture(str);
            }
    };
    
    /** MESHE */
    namespace OBJ {
        using HEADER_STACK = vector<Vec3>;
        struct IndexStack {
                HEADER_STACK normals, vertices;
                vector<Vec2> uv;
        };

        template<typename T> class ASCIIMeshLoader :
                                                     public Loader<T>,
                                                     public MemAlloc<T> {
            protected:
                map<string, GLint> headers;

            public:
                ASCIIMeshLoader(const map<string, GLint>& _headers)
                        :
                          headers(_headers) {
                }

                virtual void onNewHeader(GLint, vector<string>&) = 0;
                virtual void onHeaderArgument(c_str, GLint,
                        LOADER_ITERATOR&) = 0;

                static Vec3 getVec3D(LOADER_ITERATOR&);
                static Vec2 getVec2D(LOADER_ITERATOR&);
                
                T* load(c_str&);
                virtual ~ASCIIMeshLoader() {
                }
                
            private:
                T* selfCreateObject();
        };
        class MTLloader :
                          public ASCIIMeshLoader<MATERIALS> {
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
                MATERIALS mtl;

            public:
                MTLloader();

                void onNewHeader(GLint, vector<string>&) {
                }
                void onHeaderArgument(c_str, GLint, LOADER_ITERATOR&);
                static TextureArray* packTextures(MATERIALS&);

                /** Zwraca ostatni element!!! */
                GLuint getSize() {
                    return mtl.size();
                }
                MATERIALS* createObject() {
                    MTLloader::packTextures(mtl);
                    return new MATERIALS(mtl);
                }
                
                void releaseMemory() {
                    mtl.clear();
                }
        };
        class OBJloader :
                          public ASCIIMeshLoader<Shape3D> {
            private:
                enum HEADER
                                    : GLint {
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
                vector<Vertex4f> polygon, vertex_array;

                // MTL
                unique_ptr<MTLloader> mtl_loader;
                MATERIALS materials;
                GLint used_material = -1;

            public:
                OBJloader();

                void onNewHeader(GLint, vector<string>&);
                void onHeaderArgument(c_str, GLint, LOADER_ITERATOR&);

                Shape3D* createObject();
                void releaseMemory();

            private:
                void finalizePolygon() {
                    vertex_array.insert(vertex_array.end(), polygon.begin(),
                            polygon.end());
                    polygon.clear();
                }

                Vertex4f getVertex(LOADER_ITERATOR& iter);
        };
    }
    namespace MD5 {
        struct Joint {
                string name;
                GLint parent_index;
                POS pos;
                Vec4 orient;
        };
        struct Vertex {
                GLuint index;
                UV uv;
                GLuint start_weight;
                GLuint weight_count;
        };
        struct Weight {
                GLuint index;
                GLuint joint_index;
                GLfloat bias;
                POS pos;
        };
        struct Pack {
                string shader_path;
                deque<Vertex> vertices;
                deque<Vec3> tris;
                deque<Weight> weights;
        };
        struct MeshInfo {
                deque<Joint> joints;
                deque<Pack> meshes;

                inline Pack& lastMesh() {
                    return meshes.back();
                }
        };

        class MD5loader :
                          public Loader<Shape3D> {
            private:
                vector<Vertex4f> vertices;
                MeshInfo mesh;

            public:
                Shape3D* load(c_str&);

            private:
                void loadMeshInfo(ifstream&);
        };
    }
}

#endif
