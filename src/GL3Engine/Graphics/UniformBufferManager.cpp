#include "Effects.hpp"
#include "Mesh.hpp"

namespace GL3Engine {
    GLint UniformBufferManager::getBlockIndex(Shader* shader,
            c_str variable) {
        return glGetUniformBlockIndex(shader->getProgram(),
                variable.c_str());
    }
    void UniformBufferManager::delBuffer(GLint binding_slot) {
        if (IS_IN_MAP(ubo, binding_slot))
            glDeleteBuffers(1, &ubo[binding_slot]);
        ubo.erase(ubo.find(binding_slot));
    }

    GLint UniformBufferManager::getBlockSize(Shader* shader, c_str variable) {
        GLint block_size;
        glGetActiveUniformBlockiv(shader->getProgram(),
                getBlockIndex(shader, variable),
                GL_UNIFORM_BLOCK_DATA_SIZE,
                &block_size);
        return block_size;
    }
    GLuint UniformBufferManager::regBuffer(void* data, GLuint draw_type,
            GLint size, GLuint binding_point) {
        if (IS_IN_MAP(ubo, binding_point))
            return ubo[binding_point];
        assert(size > 0);
        GLuint handle = genGLBuffer( {
                data, static_cast<size_t>(size),
                GL_UNIFORM_BUFFER, 0, draw_type
        }, false);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, handle);
        ubo[binding_point] = handle;
        return handle;
    }
    void UniformBufferManager::changeBufferData(GLint binding_slot, void* data,
            size_t size) {
        if (!IS_IN_MAP(ubo, binding_slot) || !size || !data)
            return;
        glBindBuffer(GL_UNIFORM_BUFFER, ubo[binding_slot]);
        {
            glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
        }
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}
