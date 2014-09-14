#include "Texture.hpp"

namespace GL3Engine {
    namespace CoreMaterial {
        MaterialBufferData Material::getMaterialBufferData() const {
#define COL_DEFINE(type) \
        col[type].r(), col[type].g(), col[type].b(), col[type].a()
            return MaterialBufferData {
                    COL_DEFINE(AMBIENT),
                    COL_DEFINE(DIFFUSE),
                    COL_DEFINE(SPECULAR),
                    (GLfloat) tex_flags,
                    transparent,
                    shine,
                    0.f,
            };
        }
    }
}

