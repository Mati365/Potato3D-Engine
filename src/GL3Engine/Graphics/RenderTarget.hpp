#ifndef RENDERTARGET_HPP_
#define RENDERTARGET_HPP_
#include "Scene.hpp"
#include "Mesh.hpp"

namespace GL3Engine {
    class RenderTarget : public Node {
        protected:
            IPoint2D size;

        public:
            RenderTarget(const IPoint2D& _size = { 0, 0 })
                    :
                      size(_size) {
            }

            virtual void begin() = 0;
            virtual void end() = 0;

            virtual RenderTarget& setSize(const IPoint2D& size) {
                this->size = size;
                return *this;
            }

            virtual ~RenderTarget() {
            }
    };
    class RenderQuad : public RenderTarget {
        private:
            GLuint fbo_handle = 0,
                    depth_render_buf = 0;

            unique_ptr<Texture> color_tex, depth_tex;
            unique_ptr<Shape2D> quad;

        public:
            RenderQuad() {
            }
            RenderQuad(const IPoint2D&);
            void draw();

            void begin();
            void end();

            RenderQuad& setSize(const IPoint2D&);
            const IPoint2D& getSize() const {
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
