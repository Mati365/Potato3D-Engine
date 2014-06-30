#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
#include <functional>

#include "Mesh.hpp"

namespace GL3Engine {
    using namespace std;

    using LOADER_ITERATOR = vector<string>::iterator;

    template<typename T>
    class ASCIIloader : public Loader<T> {
        protected:
            map<string, GLint> headers;

        public:
            ASCIIloader(const map<string, GLint>& _headers)
                    :
                      headers(_headers) {
            }

            virtual void onNewHeader(GLint, vector<string>&) = 0;
            virtual void onHeaderArgument(GLint, LOADER_ITERATOR&) = 0;
            virtual T* createObject() = 0;

            static FPoint3D getVec3D(LOADER_ITERATOR& iter) {
                FPoint3D v;
                sscanf((*iter + " " + *(iter + 1) + " " + *(iter + 2)).c_str(),
                        "%f %f %f",
                        &v.X, &v.Y, &v.Z
                        );
                iter += 2;
                return v;
            }
            static FPoint2D getVec2D(LOADER_ITERATOR& iter) {
                FPoint2D v;
                sscanf((*iter + " " + *(iter + 1)).c_str(),
                        "%f %f",
                        &v.X, &v.Y
                        );
                iter++;
                return v;
            }

            T* load(ifstream& fp) {
                if (fp == nullptr)
                    return nullptr;

                string line;
                while (getline(fp, line)) {
                    if (line.empty())
                        continue;

                    GLint active_header = 0;
                    vector<string> tokens = tokenize(line, ' ');

                    if (headers.find(tokens[0]) != headers.end()) {
                        active_header = headers.find(tokens[0])->second;
                        onNewHeader(active_header, tokens);
                    } else
                        continue;
                    for (auto it = tokens.begin() + 1; it != tokens.end();
                            ++it)
                        onHeaderArgument(active_header, it);
                }
                fp.close();
                return createObject();
            }
            virtual ~ASCIIloader() {
            }
    };
    class MTLloader : public ASCIIloader<Material> {
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
            Material* mtl = new Material;

        public:
            MTLloader()
                    :
                      ASCIIloader( {
                              { "newmtl", NAME },

                              { "Ka", AMBIENT_COL },
                              { "Kd", DIFFUSE_COL },
                              { "Ks", SPECULAR_COL },

                              { "map_Ka", AMBIENT_TEX },
                              { "map_Kd", DIFFUSE_TEX },
                              { "map_Ks", SPECULAR_TEX },
                              { "map_d", BUMP_TEX },
                              { "map_bump", ALPHA_TEX },

                              { "Ns", SHINE },
                              { "D", TRANSPARENT },
                              { "Tr", TRANSPARENT }
                      }) {
            }

            void onNewHeader(GLint, vector<string>&) {
            }
            void onHeaderArgument(GLint active_header, LOADER_ITERATOR& it) {
                // Switche rozwalają formatowanie
#define DEFINE_3DVEC(header_type, mtl_type) if(active_header==header_type)mtl->mtl_type = getVec3D(it);

#define DEFINE_1DVEC(header_type, mtl_type) if(active_header==header_type)mtl->mtl_type = *it;
#define DEFINE_F_1DVEC(header_type, mtl_type) if(active_header==header_type)mtl->mtl_type = stringTo<GLfloat>(*it);
#define DEFINE_1DTEX(header_type, tex_type) if(active_header==header_type)mtl->tex[tex_type] = new Texture(*it);

                // Parametry 1 argumentowe
                DEFINE_1DVEC(NAME, name);
                DEFINE_F_1DVEC(SHINE, shine);
                DEFINE_F_1DVEC(TRANSPARENT, transparent);
                // Kolory
                DEFINE_3DVEC(AMBIENT_COL, col[Material::AMBIENT]);
                DEFINE_3DVEC(DIFFUSE_COL, col[Material::DIFFUSE]);
                DEFINE_3DVEC(SPECULAR_COL, col[Material::SPECULAR]);
                // Tekstury
                DEFINE_1DTEX(AMBIENT_TEX, Material::AMBIENT);
                DEFINE_1DTEX(DIFFUSE_TEX, Material::DIFFUSE);
                DEFINE_1DTEX(SPECULAR_TEX, Material::SPECULAR);
                DEFINE_1DTEX(ALPHA_TEX, Material::ALPHA);
                DEFINE_1DTEX(BUMP_TEX, Material::BUMP);
            }
            Material* createObject() {
                return mtl;
            }
    };
    class OBJloader : public ASCIIloader<Shape> {
        private:
            using HEADER_STACK = vector<FPoint3D>;

            struct ParseStack {
                    HEADER_STACK normals,
                            vertices;
                    vector<FPoint2D> uv;
            };
            enum HEADER
                : GLint {
                    NONE,
                VERTEX,
                NORMAL,
                TEXTURE,
                FACE,
                LOAD_MATERIAL
            };

            ParseStack indices;
            vector<Vertex> polygon, vertex_array;
            unique_ptr<MTLloader> mtl_loader;

        public:
            OBJloader()
                    :
                      ASCIIloader( {
                              { "v", VERTEX },
                              { "vn", NORMAL },
                              { "vt", TEXTURE },
                              { "f", FACE },
                              { "mtllib", LOAD_MATERIAL } }) {
                mtl_loader.reset(new MTLloader);
            }
            void onNewHeader(GLint, vector<string>&) {
                if (polygon.empty())
                    return;
                vertex_array.insert(vertex_array.end(),
                        polygon.begin(), polygon.end());
                polygon.clear();
            }
            void onHeaderArgument(GLint active_header, LOADER_ITERATOR& it) {
                switch (active_header) {
                    case LOAD_MATERIAL: {
                        ifstream fp(*it);
                        if (fp.is_open())
                            mtl_loader->load(fp);
                    }
                        break;
                    case VERTEX:
                        indices.vertices.push_back(getVec3D(it));
                        break;
                    case NORMAL:
                        indices.normals.push_back(getVec3D(it));
                        break;
                    case TEXTURE:
                        indices.uv.push_back(getVec2D(it));
                        break;
                    case FACE:
                        polygon.push_back(getVertex(it));
                        break;
                    default:
                        break;
                }
            }
            Shape* createObject() {
                return new Shape(&vertex_array[0],
                        vertex_array.size(),
                        nullptr,
                        0,
                        { 0.f, .3f, .9f, 1.f });
            }

        private:
            Vertex getVertex(LOADER_ITERATOR& iter) {
                Vertex v;
                string param = *iter;
                GLfloat args[3];

#define SHORT_COPY(size, arg_index, variable, destination_variable) \
        arrayToRaw<GLfloat,size>(indices.variable[args[arg_index] - 1].toArray(), v.destination_variable)

                switch (count(param.begin(), param.end(), '/')) {
                    // v1
                    case 0:
                        sscanf(param.c_str(), "%f", &args[0]);
                        SHORT_COPY(3, 0, vertices, pos);
                        break;
                        // v1/vt1
                    case 1:
                        sscanf(param.c_str(), "%f/%f", &args[0], &args[1]);
                        SHORT_COPY(3, 0, vertices, pos);
                        SHORT_COPY(2, 1, uv, uv);
                        break;
                    case 2:
                        if (param.find("//") == string::npos) {
                            // v1//vn1
                            sscanf(param.c_str(), "%f//%f", &args[0], &args[1]);
                            SHORT_COPY(3, 0, vertices, pos);
                            SHORT_COPY(3, 1, normals, normal);
                        } else {
                            // v1/vt1/vn1
                            sscanf(param.c_str(), "%f/%f/%f", &args[0],
                                    &args[1], &args[2]);
                            SHORT_COPY(3, 0, vertices, pos);
                            SHORT_COPY(2, 1, uv, uv);
                            SHORT_COPY(3, 2, normals, normal);
                        }
                        break;
                }

                return v;
            }
    };

    MeshLoader::MeshLoader() {
        putExtension<OBJloader>("obj");
    }
}

