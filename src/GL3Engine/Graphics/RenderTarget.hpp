#ifndef RENDERTARGET_HPP_
#define RENDERTARGET_HPP_
#include "Scene.hpp"
#include "Mesh.hpp"
#include "Billboard.hpp"

namespace GL3Engine {
    namespace SceneObject {
        class RenderTarget :
                             public CoreRenderer::Node,
                             public CoreAttrib::ScopedContainer {
            DECLARE_NODE_TYPE(RenderTarget)

            protected:
                CoreMatrix::Vec2i size;

            public:
                RenderTarget(const CoreMatrix::Vec2i& _size = { 0, 0 })
                        :
                          size(_size) {
                }
                virtual RenderTarget& setSize(const CoreMatrix::Vec2i& size) {
                    this->size = size;
                    return *this;
                }

                virtual ~RenderTarget() {
                }
        };

        using TextureMap = std::map<GLuint, std::unique_ptr<CoreMaterial::Texture>>;
        class RenderQuad :
                           public RenderTarget {
            DECLARE_NODE_TYPE(RenderQuad)

            private:
                GLuint handle = 0, depth_render_buf = 0;
                Quad quad;
                TextureMap textures;

            public:
                RenderQuad() {
                }
                RenderQuad(const CoreMatrix::Vec2i&);

                void draw() override;
                void begin() override;
                void end() override;

                RenderQuad& setSize(const CoreMatrix::Vec2i&) override;
                RenderQuad& attachTex(GLuint, CoreMaterial::Texture*,
                        GLint tex_target = -1);
                RenderQuad& setRenderFace(GLenum, GLuint);
                RenderQuad& setDrawBuffer(const std::vector<GLenum>&);

                const CoreMaterial::Texture* getAttachment(GLenum attachment) {
                    return IS_IN_MAP(textures, attachment) ?
                                                             textures[attachment]
                                                                     .get() :
                                                             nullptr;
                }
                const CoreMatrix::Vec2i& getSize() const {
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
                void passToShader() override;
        };
    }
}

#endif
