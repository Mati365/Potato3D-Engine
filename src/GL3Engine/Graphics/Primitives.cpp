#include <vector>

#include "Primitives.hpp"

namespace GL3Engine {
    Shape* Primitives::genAxis(GLfloat size) {
        if (!size % 2)
            return nullptr;
        vector<Vertex> sheet;
        GLfloat start_pos = floor(sqrt(size));

        for (GLint i = 0; i < (GLint) size; ++i) {
            sheet.push_back( {
                    { i * .5f - start_pos, 0.f, -start_pos },
                    { 0.f, 0.f, 1.f },
                    { 0.f, 0.f },
                    -1
            });
            sheet.push_back( {
                    { i * .5f - start_pos, 0.f, start_pos },
                    { 0.f, 0.f, 1.f },
                    { 0.f, 0.f },
                    -1
            });
            sheet.push_back( {
                    { start_pos, 0.f, -start_pos + i * .5f },
                    { 0.f, 0.f, 1.f },
                    { 0.f, 0.f },
                    -1
            });
            sheet.push_back( {
                    { -start_pos, 0.f, -start_pos + i * .5f },
                    { 0.f, 0.f, 1.f },
                    { 0.f, 0.f },
                    -1
            });
        }
        return new Shape(
                GL_BUFFER_DATA(&sheet[0], sheet.size() * sizeof(Vertex),
                        GL_ARRAY_BUFFER),
                GL_BUFFER_DATA(nullptr, 0, GL_ELEMENT_ARRAY_BUFFER),
                { .15f, .15f, .15f, 1.f });
    }
}
