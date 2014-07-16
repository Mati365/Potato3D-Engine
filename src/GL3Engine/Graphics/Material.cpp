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
}

