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
    TYPE_IMPORT(std, string);
    TYPE_IMPORT(std, vector);
    TYPE_IMPORT(std, cout);
    TYPE_IMPORT(std, endl);

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

