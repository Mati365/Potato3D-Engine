#ifndef IO_HPP_
#define IO_HPP_
#include "Tools.hpp"

namespace IO {
    using namespace Tools;
    
    extern size_t getFileLength(c_str&);
    
    extern void getFileContents(c_str&, vector<string>&);
    extern string getFileContents(c_str&);
    extern string getFileContents(ifstream&, GLchar);
    
    extern string removeQuotes(c_str);
    extern ifstream& readString(ifstream&, string&);
    extern string readString(ifstream&);

    template<typename T> inline T readNumber(ifstream& fp) {
        return stringTo<T>(readString(fp));
    }
    template<typename T> inline ifstream& readNumber(ifstream& fp, T& num) {
        num = readNumber<T>(fp);
        return fp;
    }

    extern char* allocString(c_str&);
}

#endif
