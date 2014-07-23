#include "Texture.hpp"

namespace GL3Engine {
    Material::Material(const Material& mat) {
        name = mat.name;
        transparent = mat.transparent;
        shine = mat.shine;
        illum_model = mat.illum_model;

        memcpy(col, mat.col, sizeof(col));
        memcpy(tex, mat.tex, sizeof(tex));
    }
    Material::Material(const Color& diffuse_color) {
        col[DIFFUSE] = diffuse_color;
    }

    MaterialBufferData Material::getMaterialBufferData() const {
#define COL_DEFINE(type) \
        col[type].r(), col[type].g(), col[type].b(), col[type].a()
#define TEX_USAGE_DEFINE(type) \
        (tex[type].empty() ? 0.f : 1.f), 0.f, 0.f, 0.f

        return MaterialBufferData {
                COL_DEFINE(AMBIENT),
                COL_DEFINE(DIFFUSE),
                COL_DEFINE(SPECULAR),

                TEX_USAGE_DEFINE(AMBIENT),
                TEX_USAGE_DEFINE(DIFFUSE),
                TEX_USAGE_DEFINE(SPECULAR),
                TEX_USAGE_DEFINE(ALPHA),
                TEX_USAGE_DEFINE(BUMP),

                transparent,
                shine,

                0.f,
                0.f
        };
    }
}

