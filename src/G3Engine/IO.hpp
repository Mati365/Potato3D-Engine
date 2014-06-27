/*
 * IO.hpp
 *
 *  Created on: 27 cze 2014
 *      Author: mateusz
 */

#ifndef IO_HPP_
#define IO_HPP_
#include "Tools.hpp"

namespace IO {
    using namespace Tools;

    inline size_t getFileLength(const string&);
    string getFileContents(const string&);
    char* allocString(const string&);
}

#endif /* IO_HPP_ */
