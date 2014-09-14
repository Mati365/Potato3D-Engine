#ifndef SCENE_HPP_
#define SCENE_HPP_
#include <stdexcept>

#include "Effects.hpp"
#include "MatrixStack.hpp"

namespace GL3Engine {
    namespace CoreInterface {
        class Drawable {
            public:
                virtual void draw()=0;
                virtual void passToShader() {
                }

                virtual ~Drawable() {
                }
        };
        class WindowEventListener {
            public:
                virtual GLboolean getMouseEvent(
                        const CoreMatrix::Vec2i&,
                        GLuint) {
                    return false;
                }
                virtual GLboolean getKeyEvent(GLchar) {
                    return false;
                }
                virtual ~WindowEventListener() {
                }
        };
    }
    namespace SceneObject {
        class RenderTarget;
    }
    namespace CoreRenderer {
        class SceneManager;

        class Node :
                     public CoreInterface::Drawable,
                     public CoreInterface::WindowEventListener,
                     public CoreAttrib::AttribContainer<GLint, Node> {
                template<typename U> friend class Batch;
                friend class SceneManager;

            public:
                enum class State {
                    NORMAL,
                    DISABLED,
                    DESTROYED
                };
                struct NodeConfig {
                        GLuint gl_render_flag;
                        State state;
                };

            protected:
                Node* parent = nullptr;
                SceneManager* scene = nullptr;
                NodeConfig config = { GL_TRIANGLES, State::NORMAL };

                CoreMatrix::MatrixStack* world = nullptr;
                CoreMatrix::Transform transform;
                CoreEffect::EffectManager effect;

            public:
                virtual void update() {
                }
                virtual void draw() override {
                }

                Node& setConfig(const NodeConfig& config) {
                    this->config = config;
                    return *this;
                }
                Node& setEffect(CoreEffect::Shader* effect) {
                    this->effect.setAttrib(effect);
                    return *this;
                }

                CoreMatrix::Transform& getTransform() {
                    return transform;
                }
                CoreEffect::EffectManager& getEffectMgr() {
                    return effect;
                }
                Node* getParentNode() const {
                    return parent;
                }
                SceneManager* getScene() const {
                    return scene;
                }

                NodeConfig& getConfig() {
                    return config;
                }
                inline GLboolean isActive() const {
                    return config.state == State::NORMAL;
                }

                virtual size_t getHash() const = 0;

#define CLASS_HASH(T) \
            Tools::constHash(#T)
#define DECLARE_NODE_TYPE(T) \
            public: \
            size_t getHash() const override { \
                return CLASS_HASH(T); \
            }
        };
        template<typename T> class Batch :
                                           public Node {
            DECLARE_NODE_TYPE(Batch)

            protected:
                std::vector<T*> objects;

            public:
                Batch& regObject(T& object) {
                    if (std::is_base_of<Node, T>::value)
                        dynamic_cast<Node*>(&object)->parent = this;
                    objects.push_back(&object);
                    return *this;
                }
        };
        class SceneManager :
                             public CoreAttrib::AttribContainer<GLint,
                                     SceneManager>,
                             public CoreInterface::WindowEventListener,
                             public CoreInterface::Drawable {
            public:
                enum class SceneFlag {
                    LIGHT_SHADER_BINDING,
                    MATERIAL_BUFFER_BINDING
                };
                using SceneFlags = std::map<SceneFlag, GLuint>;
                using NodeList = std::vector<std::unique_ptr<Node>>;

            private:
                CoreMatrix::MatrixStack world_matrix;

                NodeList nodes;
                SceneObject::RenderTarget* target = nullptr;
                SceneFlags flags = {
                        { SceneFlag::LIGHT_SHADER_BINDING, 0 },
                        { SceneFlag::MATERIAL_BUFFER_BINDING, 1 }
                };

            public:
                SceneManager(const CoreMatrix::Vec2i&);
                SceneManager(const CoreMatrix::Vec2i&, const GLuint&);

                inline NodeList::iterator begin() {
                    return nodes.begin();
                }
                inline NodeList::iterator end() {
                    return nodes.end();
                }

                SceneManager& addSceneNode(Node*);
                template<typename T>
                T& createSceneNode() {
                    static_assert(std::is_base_of<Node, T>::value, "Bad node type!");
                    Node* node = new T;
                    {
                        addSceneNode(node);
                    }
                    return *dynamic_cast<T*>(node);
                }
                SceneManager& setRenderTarget(
                        SceneObject::RenderTarget* target) {
                    this->target = target;
                    return *this;
                }

                void draw() override;
                GLboolean getMouseEvent(const CoreMatrix::Vec2i&, GLuint)
                        override;
                GLboolean getKeyEvent(GLchar) override;

#define GET_SCENE_FLAG(scene, flag) \
            scene->getSceneFlags()[CoreRenderer::SceneManager::SceneFlag::flag]
#define SET_SCENE_FLAG(scene, flag, value) \
            scene->getSceneFlags()[CoreRenderer::SceneManager::SceneFlag::flag] = value

                SceneObject::RenderTarget* getRenderTarget() {
                    return target;
                }
                SceneFlags& getSceneFlags() {
                    return flags;
                }

                CoreMatrix::MatrixStack& getWorldMatrix() {
                    return world_matrix;
                }
                SceneObject::Camera* getActiveCam() const {
                    return world_matrix.getActiveCamera();
                }
                const CoreMatrix::Vec2i& getRenderResolution() const {
                    return world_matrix.getResolution();
                }
        };
    }
}

#endif
