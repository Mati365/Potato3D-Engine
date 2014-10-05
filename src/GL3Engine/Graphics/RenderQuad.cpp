#include "RenderTarget.hpp"
#include "../Resources/Resources.hpp"

namespace GL3Engine {
    namespace SceneObject {
        TYPE_IMPORT(CoreMatrix, Vec2i);
        TYPE_IMPORT(CoreMaterial, Texture);
        TYPE_IMPORT(CoreType, Vertex2f);
        TYPE_IMPORT(std, vector);

        RenderQuad::RenderQuad(const Vec2i& _size)
                :
                  RenderTarget(_size) {
            setSize(_size);
        }

        RenderQuad& RenderQuad::attachColorTex() {
            attachTex(GL_COLOR_ATTACHMENT0,
                    new Texture(size, Texture::default_tex_flags));
            glDrawBuffer( { GL_COLOR_ATTACHMENT0 });
            return *this;
        }
        RenderQuad& RenderQuad::attachDepthTex() {
            attachTex(GL_DEPTH_ATTACHMENT,
                    new Texture(size,
                            { GL_DEPTH_COMPONENT, GL_FLOAT,
                                    Texture::default_tex_flags.flags,
                                    GL_TEXTURE_2D
                            }));
            return *this;
        }
        RenderQuad& RenderQuad::attachTex(GLuint attachment, Texture* tex,
                GLint face) {
            assert(tex);
            if (!textures.size())
                create();

            glBindFramebuffer(GL_FRAMEBUFFER, handle);
            glFramebufferTexture2D(GL_FRAMEBUFFER,
                    attachment,
                    face > 0 ? face : tex->getTexFlags().tex_type,
                    tex->getHandle(), 0);
            assert(
                    glCheckFramebufferStatus(GL_FRAMEBUFFER)
                    == GL_FRAMEBUFFER_COMPLETE);
            textures[attachment].reset(tex);
            return *this;
        }
        void RenderQuad::create() {
            if (handle)
                glDeleteFramebuffers(1, &handle);
            glGenFramebuffers(1, &handle);
            if (!effect)
                setEffect(REQUIRE_RES(CoreEffect::Shader, DEFAULT_FBO_SHADER));
        }

        RenderQuad& RenderQuad::setDrawBuffer(const vector<GLenum>& buf) {
            glDrawBuffers(buf.size(), &buf[0]);
            glBindFramebuffer(GL_FRAMEBUFFER, handle);
            return *this;
        }
        RenderQuad& RenderQuad::setRenderFace(GLenum attachment, Texture* tex,
                GLuint face) {
            if (!IS_IN_MAP(textures, attachment) || !tex)
                return *this;

            glBindFramebuffer(GL_FRAMEBUFFER, handle);
            glFramebufferTexture2D(GL_FRAMEBUFFER,
                    attachment,
                    face,
                    tex->getHandle(), 0);
            if (attachment != GL_DEPTH_ATTACHMENT)
                glDrawBuffer( { attachment });

            glViewport(0, 0, size[0], size[1]);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            assert(
                    glCheckFramebufferStatus(GL_FRAMEBUFFER)
                    == GL_FRAMEBUFFER_COMPLETE);
            return *this;
        }
        void RenderQuad::passToShader() {
            if (IS_IN_MAP(textures, GL_COLOR_ATTACHMENT0))
                effect->setUniform(GL_TEXTURE_2D,
                        "color_texture", 0,
                        textures[GL_COLOR_ATTACHMENT0]->getHandle());

            if (IS_IN_MAP(textures, GL_DEPTH_ATTACHMENT))
                effect->setUniform(GL_TEXTURE_2D,
                        "depth_texture", 1,
                        textures[GL_DEPTH_ATTACHMENT]->getHandle());
        }

        void RenderQuad::begin() {
            glBindFramebuffer(GL_FRAMEBUFFER, handle);
        }
        void RenderQuad::draw() {
            assert(effect);
            {
                passToShader();
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            }
            if (scene)
                scene->setRenderTarget(this);
        }
        void RenderQuad::end() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        RenderQuad& RenderQuad::setSize(const Vec2i& size) {
            this->size = size;
            return *this;
        }
    }
}

