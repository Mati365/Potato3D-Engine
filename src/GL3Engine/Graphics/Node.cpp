#include "Scene.hpp"

namespace GL3Engine {
    Node& Node::setShaderParam(
                               c_str variable,
                               const vector<GLfloat>& data) {
        shader_params[variable] = data;
        return *this;
    }
    void Node::update() {
        if (effect) {
            effect->begin();
            for (auto& uniform : shader_params)
                effect->setUniform(uniform.first,
                        &uniform.second[0],
                        uniform.second.size());
        }
        draw();
        if (effect)
            effect->end();
    }
}

