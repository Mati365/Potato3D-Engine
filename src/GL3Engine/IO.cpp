#include <sstream>
#include <fstream>

#include "IO.hpp"

namespace IO {
    size_t getFileLength(const string& path) {
        return ifstream(path, ios::binary | ios::ate).tellg();
    }
    string getFileContents(const string& path) {
        ifstream f(path);
        return getFileContents(f, '\n');
    }
    string getFileContents(ifstream& t, char separate) {
        string line, out;
        while(getline(t, line))
            out += line + separate;
        return out;
    }
    char* allocString(const string& str) {
        char* buf = new char[str.length()];
        for (GLuint i = 0; i < str.length(); ++i)
            buf[i] = str[i];
        return buf;
    }
}

