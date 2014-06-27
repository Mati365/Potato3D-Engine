#ifndef IO_HPP_
#define IO_HPP_
#include "Tools.hpp"

namespace IO {
    using namespace Tools;

    extern size_t getFileLength(const string&);
    extern string getFileContents(const string&);
    extern char* allocString(const string&);
}

#endif
