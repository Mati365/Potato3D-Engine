#include "Scene.hpp"
#include "RenderTarget.hpp"

namespace GL3Engine {
    void SceneManager::draw() {
        if (target)
            target->begin();
        {
            world_matrix.switchMode(MatrixStack::Mode::_3D);
            for (auto& node : nodes)
                if (node.get() != target)
                    node.get()->update();
        }
        if (target) {
            target->end();
            target->update();
        }
    }
    
#define ITER_DEF(func_name, ...) \
    for (auto& n : nodes) \
        if (n->isActive() && \
            n->func_name(__VA_ARGS__)) \
            return true; \
    return false;
    GLboolean SceneManager::getMouseEvent(const Vec2i& p, GLuint btn) {
        ITER_DEF(getMouseEvent, p, btn)
    }
    GLboolean SceneManager::getKeyEvent(GLchar c) {
        ITER_DEF(getKeyEvent, c)
    }
}

