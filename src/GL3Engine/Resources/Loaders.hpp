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
            static string putToFileName(string, c_str&);
            Shader* load(c_str&);
    };
    class Textureloader :
                          public Loader<Texture> {
        public:
            Texture* load(c_str& str) {
                return new Texture(str);
            }
    };
    
    /** MESHE */
    template<typename T> class ASCIIMeshLoader :
                                                 public Loader<T> {
        protected:
            map<string, GLint> headers;

        public:
            ASCIIMeshLoader(const map<string, GLint>& _headers)
                    :
                      headers(_headers) {
            }
            
            virtual void onNewHeader(GLint, vector<string>&) = 0;
            virtual void onHeaderArgument(c_str, GLint, LOADER_ITERATOR&) = 0;

            virtual T* createObject() = 0;
            virtual void releaseMemory() = 0;

            static FPoint3D getVec3D(LOADER_ITERATOR& iter) {
                FPoint3D v;
                sscanf((*iter + " " + *(iter + 1) + " " + *(iter + 2)).c_str(),
                        "%f %f %f", &v.X, &v.Y, &v.Z);
                iter += 2;
                return v;
            }
            static FPoint2D getVec2D(LOADER_ITERATOR& iter) {
                FPoint2D v;
                sscanf((*iter + " " + *(iter + 1)).c_str(), "%f %f", &v.X,
                        &v.Y);
                iter++;
                return v;
            }
            
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

            vector<Material*> mtl;
            vector<string> textures;

        public:
            MTLloader();

            void onNewHeader(GLint, vector<string>&) {
            }
            void onHeaderArgument(c_str, GLint, LOADER_ITERATOR&);

            /** Zwraca ostatni element!!! */
            GLuint getSize() {
                return mtl.size();
            }
            MATERIALS* createObject() {
                packTextures();
                return new MATERIALS(mtl);
            }
            
            void releaseMemory() {
                mtl.clear();
                textures.clear();
            }
            
        private:
            void packTextures();
    };
    class OBJloader :
                      public ASCIIMeshLoader<Shape3D> {
        private:
            using HEADER_STACK = vector<FPoint3D>;

            struct ParseStack {
                    HEADER_STACK normals, vertices;
                    vector<FPoint2D> uv;

                    void clear() {
                        normals.clear();
                        vertices.clear();
                        uv.clear();
                    }
            };
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

            // Dane z pliku *.obj
            ParseStack indices;
            vector<Vertex4f> polygon, vertex_array;

            // Dane z pliku *.mtl
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

#endif
