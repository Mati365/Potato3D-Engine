#include <vector>

#include "Primitives.hpp"

namespace GL3Engine {
    Mesh* Primitives::genAxis(GLfloat size) {
        if (!size % 2)
            size++;
        vector<Vertex4f> sheet;
        GLfloat start_pos = floor(sqrt(size));

        for (GLint i = 0; i < (GLint) size; ++i) {
            sheet.push_back( {
                    { i * .5f - start_pos, 0.f, -start_pos },
                    { 0.f, 1.f, 0.f },
                    { 0.f, 0.f },
                    -1
            });
            sheet.push_back( {
                    { i * .5f - start_pos, 0.f, start_pos },
                    { 0.f, 1.f, 0.f },
                    { 0.f, 0.f },
                    -1
            });
            sheet.push_back( {
                    { start_pos, 0.f, -start_pos + i * .5f },
                    { 0.f, 1.f, 0.f },
                    { 0.f, 0.f },
                    -1
            });
            sheet.push_back( {
                    { -start_pos, 0.f, -start_pos + i * .5f },
                    { 0.f, 1.f, 0.f },
                    { 0.f, 0.f },
                    -1
            });
        }
        Shape3D* shape = new Shape3D(
                {
                        &sheet[0],
                        sheet.size() * sizeof(Vertex4f),
                        GL_ARRAY_BUFFER,
                        0,
                        GL_STATIC_DRAW
                },
                {
                        nullptr,
                        0,
                        GL_ELEMENT_ARRAY_BUFFER,
                        0,
                        GL_STATIC_DRAW
                },
                { .1f, .1f, .1f, 1.f });
        return new Mesh(
                shape,
                GET_SHADER(ShaderManager::DEFAULT_MESH_SHADER));
    }
}
