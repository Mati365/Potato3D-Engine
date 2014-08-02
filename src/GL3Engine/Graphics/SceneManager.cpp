#include "Scene.hpp"
#include "RenderTarget.hpp"

namespace GL3Engine {
    void SceneManager::draw() {
        if (target)
            target->begin();
        {
            world_matrix.switchMode(MatrixStack::_3D);
            for (auto& node : nodes)
                if (node.get() != target)
                    node.get()->draw();
        }
        if (target) {
            target->end();
            target->draw();
        }
    }
}

