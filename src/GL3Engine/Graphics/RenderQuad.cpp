#include "RenderTarget.hpp"
#include "../Resources/Resources.hpp"

namespace GL3Engine {
    RenderQuad::RenderQuad(const Vec2i& _size)
            :
              RenderTarget(_size) {
        setSize(_size);
    }
    
    void RenderQuad::create() {
        if (fbo_handle)
            glDeleteFramebuffers(1, &fbo_handle);
        // FBO
        glGenFramebuffers(1, &fbo_handle);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);
        {
            // Depth buffer
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                GL_TEXTURE_2D, depth_tex->getHandle(), 0);
            }
            
            // Texture
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D, color_tex->getHandle(), 0);
            }
            assert(
                    glCheckFramebufferStatus(GL_FRAMEBUFFER)
                    == GL_FRAMEBUFFER_COMPLETE);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        // QUAD
        vector<Vertex2f> vertex_array = {
                {
                        -1.f, -1.f, 0.f, 0.f, 0.f }, {
                        1.f, -1.f, 0.f, 1.f, 0.f }, {
                        1.f, 1.f, 0.f, 1.f, 1.f }, {
                        -1.f, 1.f, 0.f, 0.f, 1.f }, };
        quad.reset(new Shape2D( {
                &vertex_array[0], vertex_array.size() * sizeof(Vertex2f),
                GL_ARRAY_BUFFER, 0, GL_STATIC_DRAW }, {
                Tile::quad_indices, 6 * sizeof(GLfloat),
                GL_ELEMENT_ARRAY_BUFFER, 0, GL_STATIC_DRAW }));
    }
    
    void RenderQuad::passToShader() {
        effect->setUniform(GL_TEXTURE_2D, "color_texture", 0,
                color_tex->getHandle()).setUniform(GL_TEXTURE_2D,
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
        scene->setRenderTarget(this);
    }
    
    void RenderQuad::begin() {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);
        glViewport(0, 0, size[0], size[1]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void RenderQuad::end() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, size[0], size[1]);
    }
    
    RenderQuad& RenderQuad::setSize(const Vec2i& size) {
        this->size = size;
        this->color_tex.reset(new Texture(size));
        this->depth_tex.reset(new Texture(size, GL_DEPTH_COMPONENT, GL_FLOAT));
        
        if (!effect)
            setEffect(REQUIRE_RES(Shader, DEFAULT_FBO_SHADER));
        create();
        return *this;
    }
}

