#include <sstream>
#include <fstream>

#include "IO.hpp"

namespace IO {
    size_t getFileLength(c_str& path) {
        return ifstream(path, ios::binary | ios::ate).tellg();
    }
    
    string getFileContents(c_str& path) {
        ifstream f(path);
        return getFileContents(f, '\n');
    }
    string getFileContents(ifstream& t, GLchar separate) {
        string line, out;
        while (getline(t, line))
            out += line + separate;
        return out;
    }
    void getFileContents(c_str& path, vector<string>& buf) {
        ifstream fp(path);
        string line;
        if (!fp)
            return;
        buf.clear();
        while (getline(fp, line))
            if (!line.empty())
                buf.push_back(line);
    }
    
    string removeQuotes(c_str str) {
        return str.substr(str.find_first_of('"') + 1, str.find_last_of('"') - 1);
    }
    ifstream& readString(ifstream& fp, string& str) {
        fp >> str;
        str = removeQuotes(str);
        return fp;
    }
    string readString(ifstream& fp) {
        string t;
        readString(fp, t);
        return t;
    }

    char* allocString(c_str& str) {
        char* buf = new char[str.length()];
        for (GLuint i = 0; i < str.length(); ++i)
            buf[i] = str[i];
        return buf;
    }
}

