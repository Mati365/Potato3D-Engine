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
    class RenderQuad :
                       public RenderTarget {
        DECLARE_NODE_TYPE(RenderQuad)

        private:
            GLuint fbo_handle = 0, depth_render_buf = 0;

            unique_ptr<Texture> color_tex, depth_tex;
            unique_ptr<Shape2D> quad;

            enum Flags {
                USE_COLOR_BUFFER = 1 << 0,
                USE_DEPTH_BUFFER = 1 << 1
            };
            GLuint flags = USE_COLOR_BUFFER | USE_DEPTH_BUFFER;

        public:
            RenderQuad() {
            }
            RenderQuad(const Vec2i&);

            void draw() override;
            void begin();
            void begin(GLuint, GLuint);
            void end();

            RenderQuad& setFlags(GLuint flags) {
                this->flags = flags;
                return *this;
            }
            RenderQuad& setSize(const Vec2i&) override;
            const Vec2i& getSize() const {
                return size;
            }
            
            GLuint getFBO() const {
                return fbo_handle;
            }
            Texture& getColorTexture() {
                return *color_tex.get();
            }
            Texture& getDepthTexture() {
                return *depth_tex.get();
            }
            
            ~RenderQuad() {
                glDeleteFramebuffers(1, &fbo_handle);
            }
            
        protected:
            void create();
            void passToShader();
    };
}

#endif
