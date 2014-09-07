#include <sstream>
#include <stdio.h>
#include <map>

#include "Tools.hpp"

namespace GL3Engine {
    GLuint operator*(GLuint number, std::function<void(void)> func) {
        for (GLuint i = 0; i < number; ++i)
            func();
        return number;
    }
}
namespace Tools {
    using std::string;
    using std::vector;
    using std::cout;
    using std::endl;

    std::map<GLenum, string> gl_errors = {
            { GL_INVALID_ENUM, "Invalid enum!" },
            { GL_INVALID_VALUE, "Invalid value!" },
            { GL_INVALID_OPERATION, "Invalid operation!" },
            { GL_INVALID_FRAMEBUFFER_OPERATION, "Invalid fbo operation!" },
            { GL_STACK_OVERFLOW, "Stack overflow!" },
            { GL_OUT_OF_MEMORY, "Out of memory!" },
            { GL_STACK_UNDERFLOW, "Stack underflow!" }
    };
    void showGLErrors() {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR)
            cout << "_GL: "
                    << (IS_IN_MAP(gl_errors, err) ? gl_errors[err] :
                                                    toString((GLint) err))
                    << endl;
    }

    std::deque<Log> Log::logs;
    void Log::putLog(Log::Flag flag, c_str str) {
        static const char* flag_caption[] = {
                "> !CRITICAL!",
                "> !WARNING!",
                "> !ERROR!"
        };
        logs.push_back( {
                flag, str });
        cout <<
                string(flag_caption[static_cast<GLint>(flag)]) + " " + str
                << endl;
        if (flag == Log::Flag::CRITICAL)
            exit(1);
    }

    std::vector<string> tokenize(c_str line, char sep) {
        vector<string> buf;
        string tok;
        std::istringstream iss(line);
        while (getline(iss, tok, sep))
            buf.push_back(tok);
        return buf;
    }
    size_t hashString(c_str str) {
        static std::hash<string> _hash;
        return _hash(str);
    }
}

