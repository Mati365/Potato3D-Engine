#ifndef PRIMITIVES_HPP_
#define PRIMITIVES_HPP_
#include "Mesh.hpp"
#include "Text.hpp"

namespace GL3Engine {
    class Primitives {
        public:
            static Mesh* genAxis(GLfloat);

            static void printText(Shader*, MatrixStack&, c_str, const FPoint3D&,
                    GLfloat,
                    Font* font = nullptr);
    };
}

#endif
