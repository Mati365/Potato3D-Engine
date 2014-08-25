#include "Mesh.hpp"

namespace GL3Engine {
    const TextureFlags Texture::default_tex_flags =
            { GL_RGBA, GL_UNSIGNED_BYTE, CLAMP_TO_EDGE | NEAREST, GL_TEXTURE_2D };

    Texture::Texture(c_str path, const TextureFlags& _flags)
            :
              flags(_flags) {
        loadTexture(path);
    }

    Texture::Texture(const TextureFlags& _flags)
            :
              flags(_flags) {
    }
    Texture::Texture(const Vec2i& _size, const TextureFlags& _flags)
            :
              flags(_flags) {
        generate(_size);
    }

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
    void Texture::generate(const Vec2i& size) {
        if (handle)
            glDeleteTextures(1, &handle);

        glGenTextures(1, &handle);
        glBindTexture(flags.tex_type, handle);
        {
            glTexImage2D(flags.tex_type, 0,
                    flags.type, size.X(), size.Y(),
                    0, flags.type, flags.bytes, nullptr);
        }
        configure();
    }
    void Texture::configure() {
        putGLTextureFlags(flags.tex_type, flags.flags);

        glGetTexLevelParameteriv(flags.tex_type, 0, GL_TEXTURE_WIDTH,
                &size[0]);
        glGetTexLevelParameteriv(flags.tex_type, 0, GL_TEXTURE_HEIGHT,
                &size[1]);
    }

    void Texture::loadTexture(c_str path) {
        if (handle)
            glDeleteTextures(1, &handle);

        handle = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
                SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO);
        configure();
    }

    // ---- TexArray
    TextureArray::TextureArray(const vector<string>& _textures,
            const TextureFlags& _flags)
            :
              textures(_textures),
              flags(_flags) {
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
            putGLTextureFlags(GL_TEXTURE_2D_ARRAY, flags.flags);
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

    // ---- CubeTex
    CubeTexture::CubeTexture(const Vec2i& size, const TextureFlags& _flags)
            :
              Texture(_flags) {
        generate(size);
    }
    void CubeTexture::generate(const Vec2i& size) {
        if (handle)
            glDeleteTextures(1, &handle);

        glGenTextures(1, &handle);
        glBindTexture(flags.tex_type, handle);
        for (GLuint i = 0; i < 6; ++i)
            glTexImage2D(flags.tex_type + i, 0,
                    flags.type, size.X(), size.Y(), 0,
                    flags.type, flags.bytes, nullptr);
        configure();
    }
}

