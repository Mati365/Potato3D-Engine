#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_
#include <SOIL/SOIL.h>

#include "Types.hpp"

namespace GL3Engine {
    using namespace Tools;
    using namespace std;
    
    extern void putGLTextureFlags(GLenum, GLuint);
    class Texture {
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

        protected:
#define DEFAULT_TEX_FLAGS CLAMP_TO_EDGE | NEAREST
            GLuint handle = 0,
                    flags = DEFAULT_TEX_FLAGS,
                    tex_type = GL_TEXTURE_2D;

            Vec2i size;

        public:
            Texture() {
            }
            Texture(c_str);
            Texture(c_str, GLuint);
            Texture(const Vec2i&,
                    GLenum type = GL_RGBA,
                    GLenum bytes = GL_UNSIGNED_BYTE,
                    GLuint flags = DEFAULT_TEX_FLAGS);

            virtual void loadTexture(c_str);
            virtual void generate(const Vec2i&, GLenum, GLenum);

            GLuint getTexType() const {
                return tex_type;
            }
            GLuint getHandle() const {
                return handle;
            }
            const Vec2i& getSize() const {
                return size;
            }
            
            virtual ~Texture() {
                glDeleteTextures(1, &handle);
            }
            
        private:
            Texture(const Texture&) {
            }
            Texture& operator=(const Texture&) {
                return *this;
            }
            
        protected:
            virtual void configure();
    };
    class TextureArray {
        private:
            GLuint handle = 0;
            vector<string> textures;

        public:
            TextureArray() {
            }
            TextureArray(const vector<string>&);

            void create();

            TextureArray& addTexture(c_str str) {
                textures.push_back(str);
                return *this;
            }
            GLuint getHandle() const {
                return handle;
            }
            
            ~TextureArray() {
                glDeleteTextures(1, &handle);
            }
    };
    class CubeTexture :
                        public Texture {
        public:
            CubeTexture(const Vec2i&,
                    GLenum type = GL_RGBA,
                    GLenum bytes = GL_UNSIGNED_BYTE,
                    GLuint flags = DEFAULT_TEX_FLAGS);

            void generate(const Vec2i&, GLenum, GLenum) override;

        private:
            // TODO: Wczytywanie paczki tekstur
            void loadTexture(c_str) override {
            }
    };
    
    /** Podzielone jako TRIANGLE!!! */
    using TILE_ITER = vector<Vertex2f>::const_iterator;
    class Tile {
        public:
            static const GLushort quad_indices[6];

        protected:
            Texture* tex;
            Vec2i cells;
            Vec2 cell_size;

            vector<Vertex2f> uv;

        public:
            Tile(Texture*, const Vec2i&);

            const Vec2i& getCells() const {
                return cells;
            }
            const Vec2& getCellSize() const {
                return cell_size;
            }
            GLuint getHandle() const {
                return tex->getHandle();
            }
            
            const Vec2i& getSize() const {
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
    using MaterialBufferData = array<GLfloat, 36>;
    struct Material {
            enum TEX_TYPE
                            : GLint {
                    AMBIENT,
                DIFFUSE,
                SPECULAR,
                ALPHA,
                BUMP
            };

            GLfloat transparent = 1.f, shine = 1.f;
            GLbyte illum_model = 0;
            Color col[SPECULAR + 1];

            string tex[BUMP + 1];
            string name;
            GLuint ubo = 0;

            std::shared_ptr<TextureArray> tex_array;

            Material() {
            }
            Material(const Material&);
            Material(const Color&);

            MaterialBufferData getMaterialBufferData() const;
    };
    
    using MATERIALS = vector<Material*>;
}

#endif
