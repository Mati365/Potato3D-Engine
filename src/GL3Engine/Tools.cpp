#include <sstream>
#include <stdio.h>

#include "Tools.hpp"

namespace Tools {
    deque<Log> Log::logs;

    void showGLErrors() {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR)
            cout << "OpenGL error: " << err << endl;
    }
    void Log::putLog(Log::Flag flag, const string& str) {
        static const char* flag_caption[] = {
                "> !CRITICAL!",
                "> !WARNING!",
                "> !ERROR!"
        };

        logs.push_back( { flag, str });
        cout << string(flag_caption[static_cast<GLint>(flag)]) + " " + str
                << endl;
    }
    vector<string> tokenize(const string& line, char sep) {
        vector<string> buf;
        istringstream iss(line);
        string tok;
        while (getline(iss, tok, sep))
            buf.push_back(tok);
        return buf;
    }
}
