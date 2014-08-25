#ifndef SCENE_HPP_
#define SCENE_HPP_
#include <stdexcept>

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
    
    class EffectManager :
                          public AttribContainer<Shader*>,
                          public ScopedContainer {
        public:
            struct EffectParam {
                    vector<GLfloat> data;
                    GLenum type;

                    GLfloat& operator[](GLuint);
            };

        private:
            map<string, EffectParam> effect_params;

        public:
            EffectManager()
                    :
                      AttribContainer(nullptr) {
            }
            EffectParam& setEffectParam(c_str, GLuint);

            void begin() const override;
            void end() const override;
    };
    class Node :
                 public Drawable,
                 public WindowEventListener {
            friend class SceneManager;
            template<typename U> friend class Batch;

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
            MatrixStack* world = nullptr;

            Transform transform;
            EffectManager effect;
            GLuint render_mode = GL_TRIANGLES;
            State state = State::NORMAL;

        public:
            virtual void update() {
            }
            virtual void draw() override {
            }

            Node& setRenderMode(GLuint render_mode) {
                this->render_mode = render_mode;
                return *this;
            }
            Transform& getTransform() {
                return transform;
            }

            Node& setEffect(Shader* effect) {
                this->effect.setAttrib(effect);
                return *this;
            }
            Node& setState(State state) {
                this->state = state;
                return *this;
            }

            EffectManager& getEffectMgr() {
                return effect;
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
            State getState() const {
                return state;
            }

            virtual inline size_t getHash()=0;

#define CLASS_HASH(T) \
            constHash(#T)
#define DECLARE_NODE_TYPE(T) \
            public: \
            inline size_t getHash() override { \
                return CLASS_HASH(T); \
            }

        private:
            void render(); // tylko na scenie + efekty
    };

    template<typename T>
    class Batch :
                  public Node {
        protected:
            vector<T*> objects;

        public:
            Batch& regObject(T& object) {
                if (is_base_of<Node, T>::value)
                    dynamic_cast<Node*>(&object)->parent = this;
                objects.push_back(&object);
                return *this;
            }
    };
    class SceneManager :
                         public Drawable,
                         public WindowEventListener {
        public:
            enum class SceneFlag {
                LIGHT_SHADER_BINDING,
                MATERIAL_BUFFER_BINDING
            };
            using SceneFlags = map<SceneFlag, GLuint>;
            using NodeList = vector<unique_ptr<Node>>;

        private:
            NodeList nodes;

            MatrixStack world_matrix;
            RenderTarget* target = nullptr;

            SceneFlags flags = {
                    { SceneFlag::LIGHT_SHADER_BINDING, 0 },
                    { SceneFlag::MATERIAL_BUFFER_BINDING, 1 }
            };

        public:
            SceneManager(const Vec2i& res)
                    :
                      world_matrix(res) {
            }
            
            inline NodeList::iterator begin() {
                return nodes.begin();
            }
            inline NodeList::iterator end() {
                return nodes.end();
            }

            SceneManager& addSceneNode(Node*);
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

#define GET_SCENE_FLAG(scene, flag) \
            scene->getSceneFlags()[SceneManager::SceneFlag::flag]
#define SET_SCENE_FLAG(scene, flag, value) \
            scene->getSceneFlags()[SceneManager::SceneFlag::flag] = value

            RenderTarget* getRenderTarget() {
                return target;
            }
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
