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
                    { 1.f, 1.f, 1.f },
                    { 0.f, 0.f }
            });
            sheet.push_back( {
                    { i * .5f - start_pos, 0.f, start_pos },
                    { 1.f, 1.f, 1.f },
                    { 0.f, 0.f }
            });
            sheet.push_back( {
                    { start_pos, 0.f, -start_pos + i * .5f },
                    { 1.f, 1.f, 1.f },
                    { 0.f, 0.f }
            });
            sheet.push_back( {
                    { -start_pos, 0.f, -start_pos + i * .5f },
                    { 1.f, 1.f, 1.f },
                    { 0.f, 0.f }
            });
        }
        return new Shape(
                &sheet[0],
                sheet.size(),
                nullptr,
                0,
                { .45f, .45f, .45f, 1.f });
    }
}
