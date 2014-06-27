/*
 * Tools.cpp
 *
 *  Created on: 27 cze 2014
 *      Author: mateusz
 */
#include "Tools.hpp"

namespace Tools {
    deque<Log> Log::logs;
    void showGLErrors() {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR)
            cout << "OpenGL error: " << err << endl;
    }
}

