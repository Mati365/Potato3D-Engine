#include "GPUutils.hpp"

namespace GL3Engine {
    namespace GPU {
        TYPE_IMPORT(std, string);
        TYPE_IMPORT(std, cout);
        TYPE_IMPORT(std, endl);

        std::map<GLenum, string> gl_errors = {
                { GL_INVALID_ENUM, "Invalid enum!" },
                { GL_INVALID_VALUE, "Invalid value!" },
                { GL_INVALID_OPERATION, "Invalid operation!" },
                { GL_INVALID_FRAMEBUFFER_OPERATION, "Invalid fbo operation!" },
                { GL_STACK_OVERFLOW, "Stack overflow!" },
                { GL_OUT_OF_MEMORY, "Out of memory!" },
                { GL_STACK_UNDERFLOW, "Stack underflow!" }
        };
        void GPUutils::showErrors() {
            GLenum err;
            while ((err = glGetError()) != GL_NO_ERROR)
                cout << "_GL: "
                        << (IS_IN_MAP(gl_errors, err) ? gl_errors[err] :
                                                        Tools::toString(
                                                                (GLint) err))
                        << endl;
        }
    }
}

