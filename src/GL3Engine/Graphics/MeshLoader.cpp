#include <vector>

#include "Mesh.hpp"

namespace GL3Engine {
    Shape* loadOBJ(FILE* fp) {
        if (fp == nullptr)
            return nullptr;

        enum Header {
            VERTEX = 1,
            NORMAL,
            UV,
            FACE
        };
        map<string, Header> headers;
        headers["v"] = VERTEX;
        headers["vn"] = NORMAL;
        headers["vt"] = UV;
        headers["f"] = FACE;

        vector<FPoint3D> vertices, normals;
        vector<FPoint2D> uv;

        vector<Vertex> vertex;
        while (!feof(fp)) {
            char header[2];
            fscanf(fp, "%3s", header);

            Header type = headers[string(header)];
            switch (type) {
                case NORMAL:
                    case VERTEX: {
                    FPoint3D v;
                    fscanf(fp, "%f %f %f\n", &v.X, &v.Y, &v.Z);
                    vertices.push_back(v);
                }
                    break;

                case FACE: {
                    GLuint v[3];
                    fscanf(fp, "%d %d %d\n", &v[0], &v[1], &v[2]);
                    for (GLint i = 0; i < 3; ++i)
                        vertex.push_back(
                                {
                                        {
                                                vertices[v[i] - 1].X,
                                                vertices[v[i] - 1].Y,
                                                vertices[v[i] - 1].Z
                                        },
                                        { 1.f, 1.f, 1.f },
                                        { 1.f, 1.f }
                                });
                }
                    break;

                default:
                    break;
            }
        }
        fclose(fp);
        return new Shape(&vertex[0],
                vertex.size(),
                nullptr,
                0,
                { 1.f, 0.f, 0.f, 1.f });
    }

    MeshLoader::MeshLoader() {
        putLoader("obj", loadOBJ);
    }
    Shape* MeshLoader::loadMesh(const string& path) throw (string) {
        string extension = path.substr(path.find('.') + 1);
        if (!IS_IN_MAP(loaders, "obj"))
            throw "Unsupported mesh file!";
        return loaders[extension](fopen(path.c_str(), "r"));
    }
}

