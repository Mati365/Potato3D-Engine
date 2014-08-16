#ifndef SCENE_HPP_
#define SCENE_HPP_
#include "Effects.hpp"
#include "MatrixStack.hpp"

namespace GL3Engine {
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
            virtual GLboolean getMouseEvent(const Vec2i&, GLuint) {
                return false;
            }
            virtual GLboolean getKeyEvent(GLchar) {
                return false;
            }
            virtual ~WindowEventListener() {
            }
    };
    
    class SceneManager;
    class RenderTarget;
    
    class Node :
                 public Drawable,
                 public WindowEventListener {
            friend class SceneManager;

        public:
            static constexpr GLfloat VIEW_DISTANCE = 6.f;
            enum class State {
                NORMAL,
                DISABLED,
                DESTROYED
            };

        protected:
            Node* parent = nullptr;
            SceneManager* scene = nullptr;

            Transform transform;
            Shader* effect = nullptr;
            GLuint render_mode = GL_TRIANGLES;
            State state = State::NORMAL;

        public:
            Node& setRenderMode(GLuint render_mode) {
                this->render_mode = render_mode;
                return *this;
            }
            Transform& getTransform() {
                return transform;
            }
            
            Node& setShaderParam(c_str, const vector<GLfloat>&, GLenum);
            Node& setEffect(Shader* effect) {
                this->effect = effect;
                return *this;
            }
            Shader* getEffect() {
                return effect;
            }
            
            Node& setParent(Node* parent) {
                this->parent = parent;
                return *this;
            }
            Node* getParentNode() const {
                return parent;
            }
            SceneManager* getScene() const {
                return scene;
            }
            
            GLboolean isActive() const {
                return state == State::NORMAL;
            }
            Node& setState(State state) {
                this->state = state;
                return *this;
            }
            State getState() const {
                return state;
            }
            
        private:
            void update();
    };
    class SceneManager :
                         public Drawable,
                         public WindowEventListener {
        public:
            enum class SceneFlag {
                LIGHT_SHADER_BINDING,
            };
            using SceneFlags = map<SceneManager::SceneFlag, GLuint>;

        private:
            vector<unique_ptr<Node>> nodes;
            MatrixStack world_matrix;
            RenderTarget* target = nullptr;

            SceneFlags flags = {
                    { SceneFlag::LIGHT_SHADER_BINDING, 0 }
            };

        public:
            SceneManager(const Vec2i& res)
                    :
                      world_matrix(res) {
            }
            
            SceneManager& addSceneNode(Node* node) {
                assert(node);
                {
                    node->scene = this;
                    nodes.push_back(unique_ptr<Node>(node));
                }
                return *this;
            }
            template<typename T>
            T& createSceneNode() {
                static_assert(is_base_of<Node, T>::value, "Bad node type!");
                Node* node = new T;
                {
                    addSceneNode(node);
                }
                return *dynamic_cast<T*>(node);
            }
            SceneManager& setRenderTarget(RenderTarget* target) {
                this->target = target;
                return *this;
            }
            
            void draw() override;
            GLboolean getMouseEvent(const Vec2i&, GLuint) override;
            GLboolean getKeyEvent(GLchar) override;

            SceneFlags& getSceneFlags() {
                return flags;
            }
            MatrixStack& getWorldMatrix() {
                return world_matrix;
            }
            Camera* getActiveCam() const {
                return world_matrix.getActiveCamera();
            }
            const Vec2i& getRenderResolution() const {
                return world_matrix.getResolution();
            }
    };
}

#endif
