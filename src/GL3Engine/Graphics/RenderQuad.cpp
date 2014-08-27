#include "RenderTarget.hpp"
#include "../Resources/Resources.hpp"

namespace GL3Engine {
    RenderQuad::RenderQuad(const Vec2i& _size)
            :
              RenderTarget(_size) {
        setSize(_size);
    }
    
    void RenderQuad::create() {
        if (handle)
            glDeleteFramebuffers(1, &handle);

        // FBO
        glGenFramebuffers(1, &handle);
        glBindFramebuffer(GL_FRAMEBUFFER, handle);
        {
            // Texture
            if (IS_SET_FLAG(USE_COLOR_BUFFER, flags)) {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D, color_tex->getHandle(), 0);
            }
            // Depth buffer
            if (IS_SET_FLAG(USE_DEPTH_BUFFER, flags)) {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                GL_TEXTURE_2D, depth_tex->getHandle(), 0);
            }
            if (flags == USE_DEPTH_BUFFER) {
                glDrawBuffer(GL_NONE);
                glReadBuffer(GL_NONE);
            } else
                glDrawBuffer(GL_COLOR_ATTACHMENT0);
            assert(
                    glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        // QUAD
        vector<Vertex2f> vertex_array = {
                { -1.f, -1.f, 0.f, 0.f, 0.f }, {
                        1.f, -1.f, 0.f, 1.f, 0.f }, {
                        1.f, 1.f, 0.f, 1.f, 1.f }, {
                        -1.f, 1.f, 0.f, 0.f, 1.f }, };
        quad.reset(
                new Shape2D(
                        { &vertex_array[0], vertex_array.size()
                                * sizeof(Vertex2f), GL_ARRAY_BUFFER, 0, GL_STATIC_DRAW },
                        { Tile::quad_indices, 6 * sizeof(GLfloat), GL_ELEMENT_ARRAY_BUFFER, 0, GL_STATIC_DRAW }));
    }
    void RenderQuad::passToShader() {
        if (IS_SET_FLAG(USE_COLOR_BUFFER, flags))
            effect->setUniform(GL_TEXTURE_2D,
                    "color_texture", 0, color_tex->getHandle());
        if (IS_SET_FLAG(USE_DEPTH_BUFFER, flags))
            effect->setUniform(GL_TEXTURE_2D,
                    "depth_texture", 1, depth_tex->getHandle());
    }
    void RenderQuad::draw() {
        assert(effect);
        {
            passToShader();
            glDisable(GL_CULL_FACE);
            glBindVertexArray(quad->getVAO());
            {
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
            }
            glBindVertexArray(0);
            glEnable(GL_CULL_FACE);
        }
        if (scene)
            scene->setRenderTarget(this);
    }
    
    void RenderQuad::begin() {
        glBindFramebuffer(GL_FRAMEBUFFER, handle);
        {
            glViewport(0, 0, size[0], size[1]);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    }
    void RenderQuad::begin(GLenum face, GLuint tex, GLuint target) {
        if (!tex)
            return;
        glBindFramebuffer(GL_FRAMEBUFFER, handle);
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, target, face, tex, 0);
            glDrawBuffer(GL_COLOR_ATTACHMENT0);
        }
    }
    void RenderQuad::end() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, size[0], size[1]);
    }
    
    RenderQuad& RenderQuad::setSize(const Vec2i& size) {
        this->size = size;

        if (IS_SET_FLAG(USE_COLOR_BUFFER, flags))
            this->color_tex.reset(new Texture(size, tex_flags));
        if (IS_SET_FLAG(USE_DEPTH_BUFFER, flags))
            this->depth_tex.reset(
                    new Texture(size,
                            { GL_DEPTH_COMPONENT, GL_FLOAT,
                                    tex_flags.flags,
                                    tex_flags.tex_type }));

        if (!effect)
            setEffect(REQUIRE_RES(Shader, DEFAULT_FBO_SHADER));
        create();
        return *this;
    }
}

