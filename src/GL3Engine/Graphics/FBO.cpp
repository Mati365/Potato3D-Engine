#include "Mesh.hpp"

namespace GL3Engine {
    FBO::FBO(const IPoint2D& _size)
            :
              size(_size),
              tex(_size) {
        create();
    }
    void FBO::create() {
        // FBO
        glGenFramebuffers(1, &fbo_handle);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);
        {
            // Depth buffer
            GLuint depth_render_buf;
            glGenRenderbuffers(1, &depth_render_buf);
            glBindRenderbuffer(GL_RENDERBUFFER, depth_render_buf);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                    size.X, size.Y);
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            {
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                GL_RENDERBUFFER, depth_render_buf);
            }
            // Texture
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                        tex.getHandle(), 0);
            }
            assert(
                    glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // QUAD
        vector<Vertex2f> vertex_array = {
                { -1.f, -1.f, 0.f, 0.f, 0.f },
                { 1.f, -1.f, 0.f, 1.f, 0.f },
                { 1.f, 1.f, 0.f, 1.f, 1.f },
                { -1.f, 1.f, 0.f, 0.f, 1.f },
        };
        quad.reset(new Shape2D(
                {
                        &vertex_array[0],
                        vertex_array.size() * sizeof(Vertex2f),
                        GL_ARRAY_BUFFER,
                        0,
                        GL_STATIC_DRAW
                },
                {
                        Tile::quad_indices,
                        6 * sizeof(GLfloat),
                        GL_ELEMENT_ARRAY_BUFFER,
                        0,
                        GL_STATIC_DRAW
                }));
    }

    void FBO::draw(MatrixStack&, GLint, Shader* effect) {
        if (effect)
            effect->setUniform(GL_TEXTURE_2D, "texture", 0,
                    tex.getHandle());

        glDisable( GL_CULL_FACE);
        glBindVertexArray(quad->getVAO());
        {
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
        }
        glBindVertexArray(0);
        glEnable( GL_CULL_FACE);
    }
    void FBO::begin() {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);
        glViewport(0, 0, size.X, size.Y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void FBO::end() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, size.X, size.Y);
    }
}

