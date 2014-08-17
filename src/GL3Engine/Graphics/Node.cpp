#include "Scene.hpp"

namespace GL3Engine {
    Node& Node::setShaderParam(
            c_str variable, const ShaderParam& param) {
        effect_params[variable] = param;
        return *this;
    }
    void Node::update() {
        if (state == State::DISABLED)
            return;
        if (effect) {
            effect->begin();
            for (auto& el : effect_params)
                effect->setUniform(el.first, &el.second.data[0],
                        el.second.data.size(), el.second.type);
        }
        draw();
        if (effect)
            effect->end();
    }
}

