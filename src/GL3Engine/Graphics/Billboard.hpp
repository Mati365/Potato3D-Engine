#ifndef BILLBOARD_HPP_
#define BILLBOARD_HPP_
#include "Mesh.hpp"

namespace GL3Engine {
    namespace SceneObject {
        class Billboard :
                          public Shape3D {
            public:
                Billboard(c_str);
        };
    }
}

#endif
