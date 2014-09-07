#ifndef PRIMITIVES_HPP_
#define PRIMITIVES_HPP_
#include "Mesh.hpp"
#include "Text.hpp"

namespace GL3Engine {
    namespace SceneObject {
        class AxisMesh :
                         public Mesh {
            public:
                AxisMesh(GLfloat size = 17.f)
                        :
                          Mesh(genAxis(size)) {
                    setConfig( { GL_LINES, Node::State::NORMAL });
                }

            private:
                static Shape3D* genAxis(GLfloat);
        };
    }
}

#endif
