#ifndef MAP_HPP_
#define MAP_HPP_
#include "../../GL3Engine/GL3Engine.hpp"

namespace Game {
    /**
     * Mapa kafelkowa z obiektami
     * dynamicznymi typu drzwi,
     * wielkość kafla 1x1x3
     */
    struct CMapHeader {
#define FILE_HEADER 'xyz'
            GLchar  file_header[3];
            GLuint  ver;
            GLuint  size[2];
    };
}

#endif
