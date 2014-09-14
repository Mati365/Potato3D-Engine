#include "GPUutils.hpp"

namespace GL3Engine {
    namespace GPU {
        GLuint Allocator::allocBuffer(
                const BufferData& data,
                GLboolean bind) {
            GLuint buffer = 0;

            glGenBuffers(1, &buffer);
            glBindBuffer(data.type, buffer);
            glBufferData(data.type, data.len, data.data, data.vbo_draw_type);
            if (!bind)
                glBindBuffer(data.type, 0);

            mem_map[buffer] = data.len;
            return buffer;
        }
        Allocator& Allocator::deallocBuffer(
                const std::initializer_list<GLuint>& handles) {
            for (auto& handle : handles) {
                glDeleteBuffers(1, &handle);
                if (IS_IN_MAP(mem_map, handle))
                    mem_map.erase(mem_map.find(handle));
            }
            return *this;
        }
        Allocator::~Allocator() {
            for (auto& el : mem_map)
                deallocBuffer({ el.first });
        }
    }
}

