#include <fstream>

#include "Loaders.hpp"
#include "../Tools.hpp"

namespace GL3Engine {
    namespace MD5 {
        NAMESPACE_IMPORT(IO);
        TYPE_IMPORT (SceneObject, Shape3D);
        TYPE_IMPORT(CoreMatrix, Vec3);
        TYPE_IMPORT(CoreType, Vertex4f);

        TYPE_IMPORT(std, string);
        TYPE_IMPORT(std, ifstream);
        TYPE_IMPORT(std, vector);

        template<typename T> ifstream& readVec(ifstream& fp, T* destination,
                GLuint count) {
            string token;
            {
                fp >> token; // (
                for (GLuint i = 0; i < count; ++i)
                    destination[i] = readNumber<GLfloat>(fp);
                fp >> token; // )
            }
            return fp;
        }

        void MD5loader::loadMeshInfo(ifstream& fp) {
            // Dla tri FPoint3D wystarczy
            struct FileInfo {
                    GLuint joint_count;
                    GLuint mesh_count;
            } info;

            enum Header {
                MD5Version,
                NumJoints,
                NumMeshes,
                Joints,
                Mesh
            };
            std::map<string, Header> headers = {
                    { "MD5Version", MD5Version },
                    { "numJoints", NumJoints },
                    { "numMeshes", NumMeshes },
                    { "joints", Joints },
                    { "mesh", Mesh }
            };
            string token;
            fp >> token;

            mesh = {};
            while (!fp.eof()) {
                if (!IS_IN_MAP(headers, token)) {
                    fp >> token;
                    continue;
                }
                switch (headers[token]) {
                    case MD5Version:
                        fp >> token;
                        if (std::stoi(token) != 10) {
                            LOG(ERROR, "Unsupported md5 version!");
                            return;
                        }
                        break;
                    case NumJoints:
                        readNumber(fp, info.joint_count);
                        break;
                    case NumMeshes:
                        readNumber(fp, info.mesh_count);
                        break;

                    case Joints:
                        fp >> token; // {
                        info.joint_count
                                * [&] {
                                    Joint joint;
                                    {
                                        readString(fp, joint.name);
                                        readNumber(fp, joint.parent_index);
                                        readVec(fp, joint.pos, 3);
                                        readVec<GLfloat>(fp, joint.orient.matrix, 3);
                                    }
                                    mesh.joints.push_back(joint);
                                    fp.ignore(fp.tellg(), '\n');
                                };
                        fp >> token; // }
                        break;
                    case Mesh: {
                        mesh.meshes.push_back( { });

                        // shader
                        fp >> token;
                        readString(fp, mesh.lastMesh().shader_path);

                        // numvertes
                        fp.ignore(fp.tellg(), '\n');
                        fp >> token;
                        GLuint count = readNumber<GLint>(fp);
                        count * [&] {
                            Vertex vertex;
                            {
                                fp >> token; // vert

                                readNumber(fp, vertex.index);
                                readVec(fp, vertex.uv, 2);
                                readNumber(fp, vertex.start_weight);
                                readNumber(fp, vertex.weight_count);
                            }
                            mesh.lastMesh().vertices.push_back(vertex);
                        };

                        // numtris
                        fp.ignore(fp.tellg(), '\n');
                        fp >> token;
                        readNumber(fp, count);
                        count * [&] {
                            Vec3 tri;
                            {
                                readVec<GLfloat>(fp, tri.matrix, 3);
                            }
                            mesh.lastMesh().tris.push_back(tri);
                        };

                        // numweights
                        fp.ignore(fp.tellg(), '\n');
                        fp >> token;
                        readNumber(fp, count);
                        count * [&] {
                            Weight weight;
                            {
                                fp >> token; // weight

                                readNumber(fp, weight.index);
                                readNumber(fp, weight.joint_index);
                                readNumber(fp, weight.bias);
                                readVec(fp, weight.pos, 3);
                            }
                            mesh.lastMesh().weights.push_back(weight);
                        };
                    }
                        break;
                };
                fp >> token;
            }
        }
        Shape3D* MD5loader::load(c_str path) {
            ifstream fp(path, std::ios::in | std::ios::binary);
            if (!fp)
                return nullptr;
            loadMeshInfo(fp);
            fp.close();

            // Łączenie
            CoreMaterial::Materials materials;
            vector<Vertex4f> vertex_array;
            return new Shape3D(
                    { &vertex_array[0], vertex_array.size() * sizeof(Vertex4f), GL_ARRAY_BUFFER, 0, GL_STATIC_DRAW },
                    { nullptr, 0, GL_ELEMENT_ARRAY_BUFFER, 0, GL_STATIC_DRAW },
                    { },
                    materials);
        }
    }
}
