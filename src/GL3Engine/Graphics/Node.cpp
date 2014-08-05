#include "Scene.hpp"

namespace GL3Engine {
    Node& Node::setShaderParam(
                               c_str variable,
                               const vector<GLfloat>& data,
                               GLenum type) {
        effect->setUniform(variable, &data[0], data.size(), type);
        return *this;
    }
    void Node::update() {
        if (effect)
            effect->begin();
        draw();
        if (effect)
            effect->end();
    }
}

