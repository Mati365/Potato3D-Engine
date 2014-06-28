#include <vector>
#include <algorithm>
#include <sstream>
#include <map>

#include "Mesh.hpp"

namespace GL3Engine {
    using namespace std;

    using LOADER_ITERATOR = vector<string>::iterator;

    FPoint3D getVec3D(LOADER_ITERATOR& iter) {
        FPoint3D v;
        sscanf((*iter + " " + *(iter + 1) + " " + *(iter + 2)).c_str(),
                "%f %f %f",
                &v.X, &v.Y, &v.Z
                );
        iter += 2;
        return v;
    }
    FPoint2D getVec2D(LOADER_ITERATOR& iter) {
        FPoint2D v;
        sscanf((*iter + " " + *(iter + 1)).c_str(),
                "%f %f",
                &v.X, &v.Y
                );
        iter += 1;
        return v;
    }
    Shape* loadOBJ(ifstream& fp) {
        if (fp == nullptr)
            return nullptr;
        // Parsowanie
        using TEMP_STACK = vector<FPoint3D>;
        TEMP_STACK normals, vertices, uv, faces;
        TEMP_STACK* active_stack = nullptr;

        // Obsługiwane typy
        map<string, TEMP_STACK*> temps;
        temps["v"] = &vertices;
        temps["vn"] = &normals;
        temps["vt"] = &uv;
        temps["f"] = &faces;

        // Parsowanie
        vector<Vertex> vertex_array;
        vector<string> tokens = tokenize(IO::getFileContents(fp, ' '), ' ');
        for (auto it = tokens.begin(); it != tokens.end(); ++it) {
            if (temps.find(*it) != temps.end())
                active_stack = temps[*it];

            else if ((*it)[0] != '-' && !isdigit((*it)[0]))
                active_stack = nullptr;

            else if (active_stack == &uv)
                active_stack->push_back(getVec2D(it));
            else if (active_stack == &vertices)
                active_stack->push_back(getVec3D(it));
            else if (active_stack == &faces) {
                Vertex vertex;

                arrayToRaw<GLfloat, 3>(
                        vertices[stringTo<GLint>(*it) - 1].toArray(),
                        vertex.pos);

                vertex_array.push_back(vertex);
            }
        }
        return new Shape(&vertex_array[0],
                vertex_array.size(),
                nullptr,
                0,
                { 1.f, 0.f, .5f, 1.f });
    }

    MeshLoader::MeshLoader() {
        putLoader("obj", loadOBJ);
    }
    Shape* MeshLoader::loadMesh(const string& path) throw (string) {
        string extension = path.substr(path.find('.') + 1);
        if (!IS_IN_MAP(loaders, "obj"))
            throw "Unsupported mesh file!";
        ifstream file(path);
        if (!file.is_open())
            throw "File not found!";
        return loaders[extension](file);
    }
}

