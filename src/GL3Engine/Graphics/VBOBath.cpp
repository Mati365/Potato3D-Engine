#include "./Mesh.hpp"

namespace GL3Engine {
    VBOBath::VBOBath(GLsizeiptr bytes) {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        vbo = genGLBuffer( {
                nullptr,
                bytes,
                GL_ARRAY_BUFFER
        });

        indices = genGLBuffer( {
                nullptr,
                indices,
                GL_ELEMENT_ARRAY_BUFFER
        });

        glBindVertexArray(0);
    }

    VAO_BATH_PTR VBOBath::putBuffers(const GL_BUFFER_DATA& vbo_ptr,
            const GL_BUFFER_DATA& indices_ptr) {
        VAO_BATH_PTR ptr;

        assignBuffer(&ptr.vbo_offset, &ptr.vbo_length, vbo_ptr, vbo,
                &last_vbo_offset);
        assignBuffer(&ptr.indices_offset, &ptr.indices_length,
                indices_ptr, indices,
                &last_index_offset);
        return ptr;
    }
    void VBOBath::assignBuffer(GLsizeiptr* ret_offset,
            GLsizeiptr* ret_length, const GL_BUFFER_DATA& data,
            GLint gl_buffer, GLsizeiptr* buf_offset) {
        if (data.data == nullptr)
            return;

        *ret_offset = *buf_offset;
        *ret_length = data.len;

        glBindBuffer(data.type, gl_buffer);
        glBufferSubData(data.type, *buf_offset, data.len, data.data);
        glBindBuffer(data.type, 0);

        *buf_offset += data.len;
    }
}

