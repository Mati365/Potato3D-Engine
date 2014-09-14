#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_
#include <SOIL/SOIL.h>

#include "Types.hpp"

namespace GL3Engine {
    namespace CoreMaterial {
        extern void putGLTextureFlags(GLenum, GLuint);

        struct TextureFlags {
                GLenum type;
                GLenum bytes;
                GLuint flags;
                GLenum tex_type;
        };
        class Texture :
                        public CoreInterface::NonCopyable {
            public:
                enum Flags {
                    MIPMAP_NEAREST = 1 << 1,
                    MIPMAP_LINEAR = 1 << 2,

                    CLAMP = 1 << 3,
                    CLAMP_TO_EDGE = 1 << 4,
                    REPEAT = 1 << 5,

                    LINEAR = 1 << 6,
                    NEAREST = 1 << 7
                };
                static const TextureFlags default_tex_flags;

            protected:
                TextureFlags flags = default_tex_flags;
                GLuint handle = 0;
                CoreMatrix::Vec2i size;

            public:
                Texture() {
                }
                Texture(c_str, const TextureFlags& _flags = default_tex_flags);

                Texture(const TextureFlags&);
                Texture(const CoreMatrix::Vec2i&,
                        const TextureFlags& _flags = default_tex_flags);

                virtual void loadTexture(c_str);
                virtual void generate(const CoreMatrix::Vec2i&);

                const TextureFlags& getTexFlags() const {
                    return flags;
                }
                GLuint getHandle() const {
                    return handle;
                }
                const CoreMatrix::Vec2i& getSize() const {
                    return size;
                }

                virtual ~Texture() {
                    glDeleteTextures(1, &handle);
                }

            protected:
                virtual void configure();
        };
        class CubeTexture :
                            public Texture {
            public:
                static constexpr GLuint CUBE_TEX_FACES = 6;

            public:
                CubeTexture(const CoreMatrix::Vec2i&,
                        const TextureFlags& _flags = Texture::default_tex_flags);
                void generate(const CoreMatrix::Vec2i&) override;

            private:
                // TODO: Wczytywanie paczki tekstur
                void loadTexture(c_str) override {
                }
        };

        using PathList = std::vector<std::string>;
        class TextureArray :
                             public CoreInterface::NonCopyable {
            private:
                GLuint handle = 0;
                PathList textures;
                TextureFlags flags = Texture::default_tex_flags;

            public:
                TextureArray() {
                }
                TextureArray(const PathList&,
                        const TextureFlags& _flags = Texture::default_tex_flags);

                void create();

                TextureArray& addTexture(c_str str) {
                    textures.push_back(str);
                    return *this;
                }
                GLuint getHandle() const {
                    return handle;
                }
                const TextureFlags& getTexFlags() const {
                    return flags;
                }

                ~TextureArray() {
                    glDeleteTextures(1, &handle);
                }
        };

        /** Podzielone jako TRIANGLE!!! */
        using TileIterator = std::vector<CoreType::Vertex2f>::const_iterator;
        class Tile {
            public:
                static const GLushort quad_indices[6];

            protected:
                Texture* tex;

                CoreMatrix::Vec2i cells;
                CoreMatrix::Vec2 cell_size;
                std::vector<CoreType::Vertex2f> uv;

            public:
                Tile(Texture*, const CoreMatrix::Vec2i&);

                const CoreMatrix::Vec2i& getCells() const {
                    return cells;
                }
                const CoreMatrix::Vec2& getCellSize() const {
                    return cell_size;
                }
                GLuint getHandle() const {
                    return tex->getHandle();
                }

                const CoreMatrix::Vec2i& getSize() const {
                    return tex->getSize();
                }
                const Texture* getTexture() const {
                    return tex;
                }

            private:
                void tokenize();
        };

        /** Zwraca tablice bo Material nie
         * sklada sie tylko z danych dla glsl
         */
        using MaterialBufferData = std::array<GLfloat, 16>;
        struct Material :
                          public CoreInterface::NonCopyable {
                enum TEX_TYPE
                    :GLint {
                        AMBIENT,
                    DIFFUSE,
                    SPECULAR,
                    ALPHA,
                    BUMP
                };

                std::string name = "";
                GLfloat transparent = 1.f, shine = 1.f;
                CoreType::Color col[SPECULAR + 1] = {
                        { 1.f, 1.f, 1.f, 1.f },
                        { 1.f, 1.f, 1.f, 1.f },
                        { 1.f, 1.f, 1.f, 1.f }
                };
                GLuint tex_flags = 0;
                std::shared_ptr<TextureArray> tex_array = std::shared_ptr
                        < TextureArray > (new TextureArray);

                MaterialBufferData getMaterialBufferData() const;
        };

        using Materials = std::vector<Material*>;
    }
}

#endif
