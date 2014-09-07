#ifndef IO_HPP_
#define IO_HPP_
#include <vector>
#include <string>

#include "Tools.hpp"

namespace IO {
    extern size_t getFileLength(c_str&);
    
    extern void getFileContents(c_str&, std::vector<std::string>&);
    extern std::string getFileContents(c_str&);
    extern std::string getFileContents(std::ifstream&, GLchar);
    
    extern std::string removeQuotes(c_str);
    extern std::ifstream& readString(std::ifstream&, std::string&);
    extern std::string readString(std::ifstream&);

    template<typename T> inline T readNumber(std::ifstream& fp) {
        return Tools::stringTo<T>(readString(fp));
    }
    template<typename T> inline std::ifstream& readNumber(std::ifstream& fp,
            T& num) {
        num = readNumber<T>(fp);
        return fp;
    }

    extern char* allocString(c_str&);
}

#endif
