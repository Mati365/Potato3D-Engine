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
    vector<string> tokenize(const string& line, char sep) {
        vector<string> buf;
        istringstream iss(line);
        string tok;
        while (getline(iss, tok, sep))
            buf.push_back(tok);
        return buf;
    }
}

