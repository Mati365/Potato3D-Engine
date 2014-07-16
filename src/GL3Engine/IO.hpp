#ifndef IO_HPP_
#define IO_HPP_
#include "Tools.hpp"

namespace IO {
    using namespace Tools;

    extern size_t getFileLength(c_str&);

    extern void getFileContents(c_str&, vector<string>&);
    extern string getFileContents(c_str&);
    extern string getFileContents(ifstream&, char);

    extern char* allocString(c_str&);
}

#endif
