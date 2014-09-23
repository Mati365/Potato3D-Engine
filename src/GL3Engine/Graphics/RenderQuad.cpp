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

        RenderQuad& RenderQuad::attachTex(GLuint attachment, Texture* tex,
                GLint tex_target) {
            assert(tex);

            glBindFramebuffer(GL_FRAMEBUFFER, handle);
            glFramebufferTexture2D(GL_FRAMEBUFFER,
                    attachment,
                    tex_target > 0 ? tex_target : tex->getTexFlags().tex_type,
                    tex->getHandle(), 0);

            assert(
                    glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
            textures[attachment].reset(tex);
            return *this;
        }
        void RenderQuad::create() {
            if (handle)
                glDeleteFramebuffers(1, &handle);
            glGenFramebuffers(1, &handle);
        }

        RenderQuad& RenderQuad::setDrawBuffer(const vector<GLenum>& buf) {
            glDrawBuffers(buf.size(), &buf[0]);
            glBindFramebuffer(GL_FRAMEBUFFER, handle);
            return *this;
        }
        RenderQuad& RenderQuad::setRenderFace(GLenum attachment, GLuint face) {
            if (!IS_IN_MAP(textures, attachment))
                return *this;

            glBindTexture(textures[attachment]->getTexFlags().tex_type, 0);

            glBindFramebuffer(GL_FRAMEBUFFER, handle);
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, face,
                    textures[attachment]->getHandle(), 0);
            glDrawBuffer(attachment);

            glViewport(0, 0, size[0], size[1]);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            assert(
                    glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
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
            {
                glViewport(0, 0, size[0], size[1]);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }
        }
        void RenderQuad::draw() {
            assert(effect);
            {
                passToShader();
                glDisable(GL_CULL_FACE);
                glBindVertexArray(quad.getVAO());
                {
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
                }
                glBindVertexArray(0);
                glEnable(GL_CULL_FACE);
            }
            if (scene)
                scene->setRenderTarget(this);
        }
        void RenderQuad::end() {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, size[0], size[1]);
        }

        RenderQuad& RenderQuad::setSize(const Vec2i& size) {
            this->size = size;

            if (!effect)
                setEffect(REQUIRE_RES(CoreEffect::Shader, DEFAULT_FBO_SHADER));
            create();
            if (!IS_IN_MAP(textures, GL_COLOR_ATTACHMENT0)) {
                attachTex(GL_COLOR_ATTACHMENT0,
                        new Texture(size, Texture::default_tex_flags));
                glDrawBuffer( { GL_COLOR_ATTACHMENT0 });
            }
            if (!IS_IN_MAP(textures, GL_DEPTH_ATTACHMENT)) {
                CoreMaterial::TextureFlags depth_flags =
                        { GL_DEPTH_COMPONENT, GL_FLOAT,
                                Texture::default_tex_flags.flags,
                                GL_TEXTURE_2D };
                attachTex(GL_DEPTH_ATTACHMENT, new Texture(size, depth_flags));
            }
            return *this;
        }
    }
}

