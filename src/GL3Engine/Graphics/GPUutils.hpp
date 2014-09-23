#ifndef GPUUTILS_HPP_
#define GPUUTILS_HPP_
#include <map>

#include "Types.hpp"

namespace GL3Engine {
    namespace GPU {
        struct BufferData {
                const GLvoid* data;
                size_t len;
                GLint type;

                GLuint offset;
                GLuint vbo_draw_type;
        };
        class Allocator :
                          public CoreInterface::Singleton<Allocator> {
            private:
                std::map<GLuint, size_t> mem_map;

            public:
                GLuint allocBuffer(
                        const BufferData&,
                        GLboolean bind = false);
                Allocator& deallocBuffer(const std::initializer_list<GLuint>&);

                ~Allocator();
        };
        class GPUutils {
            public:
                static void showErrors();
        };
    }
}

#endif
