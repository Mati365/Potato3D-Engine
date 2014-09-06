#ifndef RENDERTARGET_HPP_
#define RENDERTARGET_HPP_
#include "Scene.hpp"
#include "Mesh.hpp"

namespace GL3Engine {
    class RenderTarget :
                         public Node {
        DECLARE_NODE_TYPE(RenderTarget)

        protected:
            Vec2i size;

        public:
            RenderTarget(const Vec2i& _size = { 0, 0 })
                    :
                      size(_size) {
            }
            
            virtual void begin() = 0;
            virtual void end() = 0;

            virtual RenderTarget& setSize(const Vec2i& size) {
                this->size = size;
                return *this;
            }
            
            virtual ~RenderTarget() {
            }
    };

    using TextureMap = map<GLuint, unique_ptr<Texture>>;
    class RenderQuad :
                       public RenderTarget {
        DECLARE_NODE_TYPE(RenderQuad)

        private:
            GLuint handle = 0, depth_render_buf = 0;
            unique_ptr<Shape2D> quad;
            TextureMap textures;

        public:
            RenderQuad() {
            }
            RenderQuad(const Vec2i&);

            void draw() override;
            void begin();
            void end();

            RenderQuad& setSize(const Vec2i&) override;
            RenderQuad& attachTex(GLuint, Texture*, GLint tex_target = -1);
            RenderQuad& setRenderFace(GLenum, GLuint);
            RenderQuad& setDrawBuffer(const vector<GLenum>&);

            const Texture* getAttachment(GLenum attachment) {
                return IS_IN_MAP(textures, attachment) ? textures[attachment]
                                                                 .get() :
                                                         nullptr;
            }
            const Vec2i& getSize() const {
                return size;
            }
            GLuint getHandle() const {
                return handle;
            }
            
            ~RenderQuad() {
                glDeleteFramebuffers(1, &handle);
            }
            
        protected:
            void create();
            void passToShader();
    };
}

#endif
