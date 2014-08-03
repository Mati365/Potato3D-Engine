#include <SOIL/SOIL.h>
#include <math.h>

#include "Mesh.hpp"

namespace GL3Engine {
    void putGLTextureFlags(GLenum type) {
        glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    Texture::Texture(const string& path) {
        loadTexture(path);
    }
    Texture::Texture(const IPoint2D& size, GLenum type, GLenum bytes) {
        generate(size, type, bytes);
    }

    void Texture::generate(const IPoint2D& size, GLenum type, GLenum bytes) {
        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D, handle);
        {
            glTexImage2D(GL_TEXTURE_2D, 0,
                    type, size.X, size.Y,
                    0, type, bytes, 0);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
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
                        SOIL_FLAG_POWER_OF_TWO);
        configure();
    }

    void Texture::configure() {
        glBindTexture(GL_TEXTURE_2D, handle);
        {
            putGLTextureFlags(GL_TEXTURE_2D);
            if (IS_SET_FLAG(flags, USE_MIPMAP_NEAREST | USE_MIPMAP_LINEAR)) {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        IS_SET_FLAG(flags, USE_MIPMAP_NEAREST) ?
                        GL_LINEAR_MIPMAP_NEAREST :
                                                                 GL_LINEAR_MIPMAP_LINEAR);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
        }
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH,
                &size.X);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT,
                &size.Y);
    }

    // --------- TextureArray
    TextureArray::TextureArray(const vector<string>& _textures)
            :
              textures(_textures) {
        create();
    }
    void TextureArray::create() {
        struct _TEX {
                GLuchar* data = nullptr;
                IPoint2D size;
                GLint mipmaps;
        };
        _TEX* data = new _TEX[textures.size()];
        IPoint2D biggest;

        for (GLuint i = 0; i < textures.size(); ++i) {
            _TEX tex;
            {
                tex.data = SOIL_load_image(textures[i].c_str(), &tex.size.X,
                        &tex.size.Y,
                        &tex.mipmaps,
                        SOIL_LOAD_RGBA);
            }
            data[i] = tex;
            if (tex.size > biggest)
                biggest = tex.size;
        }
        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D_ARRAY, handle);
        {
            putGLTextureFlags(GL_TEXTURE_2D_ARRAY);
            glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_GENERATE_MIPMAP, GL_TRUE);
        }
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, biggest.X, biggest.Y,
                textures.size());
        for (GLuint i = 0; i < textures.size(); ++i) {
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, data[i].size.X,
                    data[i].size.Y, 1, GL_RGBA, GL_UNSIGNED_BYTE,
                    data[i].data);
            SOIL_free_image_data(data[i].data);
        }
        safeDelete(data, true);
    }

    // --------- Tile
    const GLushort Tile::quad_indices[6] = {
            1, 0, 3,
            1, 3, 2
    };

    Tile::Tile(Texture* _tex, IPoint2D _cells)
            :
              tex(_tex),
              cells(_cells),
              cell_size(1.0 / _cells.X, 1.0 / _cells.Y) {
        tokenize();
    }
    void Tile::tokenize() {
        uv.clear();
        for (GLfloat i = 0; i < 1.0; i += cell_size.Y)
            for (GLfloat j = 0; j < 1.0; j += cell_size.X) {
                uv.push_back( {
                        { 0.f, cell_size.Y, 0.f },
                        { j, i }
                });
                uv.push_back( {
                        { cell_size.X, cell_size.Y, 0.f },
                        { j + cell_size.X, i }
                });
                uv.push_back( {
                        { cell_size.X, 0.f, 0.f },
                        { j + cell_size.X, i + cell_size.Y }
                });
                uv.push_back( {
                        { 0.f, 0.f, 0.f },
                        { j, i + cell_size.Y }
                });
            }
    }
}

