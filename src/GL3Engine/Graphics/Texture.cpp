#include <SOIL/SOIL.h>

#include "Mesh.hpp"

namespace GL3Engine {
    Texture::Texture(const string& path) {
        loadTexture(path);
        configure();
    }
    void Texture::loadTexture(const string& path) {
        if (handle != 0)
            glDeleteTextures(1, &handle);
        handle = SOIL_load_OGL_texture
                (
                        path.c_str(),
                        SOIL_LOAD_AUTO,
                        SOIL_CREATE_NEW_ID,
                        SOIL_FLAG_MIPMAPS |
                                SOIL_FLAG_NTSC_SAFE_RGB |
                                SOIL_FLAG_COMPRESS_TO_DXT
                                );
    }
    void Texture::configure() {
        glBindTexture(GL_TEXTURE_2D, handle);

        glTexParameteri(GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
        GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);

        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH,
                &size.X);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT,
                &size.Y);
    }
}

