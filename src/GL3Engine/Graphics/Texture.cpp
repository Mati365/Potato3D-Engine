#include <SOIL/SOIL.h>
#include <math.h>

#include "Mesh.hpp"

namespace GL3Engine {
    void putGLTextureFlags(GLenum type, GLuint flags) {
        {
            GLenum mag_filter =
            IS_SET_FLAG(flags, Texture::NEAREST) ? GL_NEAREST :
            GL_LINEAR;
            GLboolean use_mipmaps = IS_SET_FLAG(flags,
                    Texture::MIPMAP_NEAREST | Texture::MIPMAP_LINEAR);
            if (use_mipmaps)
                mag_filter =
                        IS_SET_FLAG(flags, Texture::MIPMAP_NEAREST) ? GL_NEAREST_MIPMAP_NEAREST :
                                                                      GL_LINEAR_MIPMAP_LINEAR;
            
            glTexParameteri(type, GL_TEXTURE_MIN_FILTER, mag_filter);
            glTexParameteri(type, GL_TEXTURE_MAG_FILTER, mag_filter);
            
            if (use_mipmaps) {
                glTexParameteri(type, GL_GENERATE_MIPMAP, true);
                glGenerateMipmap(type);
            }
        }
        {
            GLenum wrap_filter =
            IS_SET_FLAG(flags, Texture::CLAMP) ? GL_CLAMP :
            IS_SET_FLAG(flags, Texture::CLAMP_TO_EDGE) ? GL_CLAMP_TO_EDGE :
            GL_REPEAT;
            
            glTexParameteri(type, GL_TEXTURE_WRAP_S, wrap_filter);
            glTexParameteri(type, GL_TEXTURE_WRAP_T, wrap_filter);
        }
    }
    
    Texture::Texture(c_str path) {
        loadTexture(path);
    }
    Texture::Texture(c_str path, GLuint flags)
            :
              Texture(path) {
        this->flags = flags;
    }
    Texture::Texture(
            const Vec2i& _size, GLenum _type, GLenum _bytes, GLuint _flags)
            :
              flags(_flags) {
        generate(_size, _type, _bytes);
    }
    
    void Texture::generate(const Vec2i& size, GLenum type, GLenum bytes) {
        if (handle)
            glDeleteTextures(1, &handle);
        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D, handle);
        {
            glTexImage2D(GL_TEXTURE_2D, 0,
                    type, size.X(), size.Y(),
                    0, type, bytes, 0);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        configure();
    }
    void Texture::loadTexture(c_str path) {
        if (handle)
            glDeleteTextures(1, &handle);
        handle = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
                SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO);
        configure();
    }
    
    void Texture::configure() {
        glBindTexture(GL_TEXTURE_2D, handle);
        {
            putGLTextureFlags(GL_TEXTURE_2D, flags);
        }
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &size[0]);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &size[1]);
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
                Vec2i size = { 0, 0 };
                GLint mipmaps;
        };
        _TEX* data = new _TEX[textures.size()];
        Vec2i max_size = { 0, 0 };
        
        for (GLuint i = 0; i < textures.size(); ++i) {
            _TEX tex;
            {
                tex.data = SOIL_load_image(textures[i].c_str(),
                        &tex.size[0], &tex.size[1],
                        &tex.mipmaps, SOIL_LOAD_RGBA);
            }
            data[i] = tex;

            if (tex.size[0] * tex.size[0] >
                    max_size[0] * tex.size[0])
                max_size = tex.size;
        }

        glGenTextures(1, &handle);
        glBindTexture(GL_TEXTURE_2D_ARRAY, handle);
        {
            putGLTextureFlags(GL_TEXTURE_2D_ARRAY,
                    Texture::NEAREST | Texture::REPEAT);
        }
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8,
                max_size[0], max_size[1],
                textures.size());
        for (GLuint i = 0; i < textures.size(); ++i) {
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
                    0, 0, 0, i,
                    data[i].size[0], data[i].size[1], 1,
                    GL_RGBA, GL_UNSIGNED_BYTE, data[i].data);
            SOIL_free_image_data(data[i].data);
        }
        safeDelete(data, true);
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }
    
    // --------- Tile
    const GLushort Tile::quad_indices[6] = { 1, 0, 3, 1, 3, 2 };
    
    Tile::Tile(Texture* _tex, const Vec2i& _cells)
            :
              tex(_tex),
              cells(_cells),
              cell_size( { 1.f / _cells.X(), 1.f / _cells.Y() }) {
        tokenize();
    }
    void Tile::tokenize() {
        uv.clear();
        for (GLfloat i = 0; i < 1.0; i += cell_size[1])
            for (GLfloat j = 0; j < 1.0; j += cell_size[0]) {
                uv.push_back( {
                        { 0.f, cell_size[1], 0.f },
                        { j, i } });
                uv.push_back( {
                        { cell_size[0], cell_size[1], 0.f },
                        { j + cell_size[0], i } });
                uv.push_back( {
                        { cell_size[0], 0.f, 0.f },
                        { j + cell_size[0], i + cell_size[1] } });
                uv.push_back( {
                        { 0.f, 0.f, 0.f },
                        { j, i + cell_size[1] } });
            }
    }
}

