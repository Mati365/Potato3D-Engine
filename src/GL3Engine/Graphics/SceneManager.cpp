#include "Scene.hpp"
#include "RenderTarget.hpp"

namespace GL3Engine {
    namespace CoreRenderer {
        TYPE_IMPORT(CoreMatrix, Vec2i);

        SceneManager::SceneManager(const Vec2i& res, const GLuint& conf)
                :
                  world_matrix(res) {
            setAttrib(conf);
        }
        SceneManager::SceneManager(const Vec2i& res)
                :
                  SceneManager(res, CoreMatrix::MatrixStack::_3D) {
        }

        void SceneManager::draw() {
            for (auto& node : nodes)
                node.get()->update();
            if (target)
                target->begin();
            {
                GLint last_used_shader = -1;
                world_matrix.switchMode(attrib);
                for (auto& node : nodes) {
                    CoreEffect::Shader* shader =
                            node->getEffectMgr().getAttrib();
                    if (node.get() == target
                            || !node->isActive())
                        continue;
                    if (shader && shader->getProgram() != last_used_shader) {
                        last_used_shader = shader->getProgram();
                        shader->begin();
                    }
                    node.get()->draw();
                }
            }
            if (target) {
                target->end();
                target->getEffectMgr().begin();
                target->draw();
            }
        }
        SceneManager& SceneManager::addSceneNode(Node* node) {
            assert(node);
            {
                node->scene = this;
                node->world = &world_matrix;
                nodes.push_back(std::unique_ptr<Node>(node));
            }
            return *this;
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
}

