#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_
#include "Types.hpp"

namespace GL3Engine {
    using namespace Tools;
    using namespace std;

    class Texture {
        public:
            enum Flag
                : GLuint {
                    USE_MIPMAP_NEAREST = 1 << 1,
                USE_MIPMAP_LINEAR = 1 << 2
            };

        private:
            GLuint handle = 0, flags = 0;
            IPoint2D size;

        public:
            Texture() {
            }
            Texture(const string&);
            Texture(const string& _path, GLuint _flags)
                    :
                      Texture(_path) {
                flags = _flags;
            }

            void loadTexture(const string&);

            GLuint getHandle() const {
                return handle;
            }
            const IPoint2D& getSize() const {
                return size;
            }

            ~Texture() {
                glDeleteTextures(1, &handle);
            }

        private:
            void configure();
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

            inline void addTexture(c_str str) {
                textures.push_back(str);
            }
            GLuint getHandle() const {
                return handle;
            }
    };

    /** Podzielone jako TRIANGLE!!! */
    using TILE_ITER = vector<Vertex2f>::const_iterator;
    class Tile {
        public:
            static const GLushort indices[6];

        protected:
            Texture* tex;
            IPoint2D cells;
            FPoint2D cell_size;

            vector<Vertex2f> uv;

        public:
            Tile(Texture*, IPoint2D);

            const IPoint2D& getCells() const {
                return cells;
            }
            const FPoint2D& getCellSize() const {
                return cell_size;
            }
            GLuint getHandle() const {
                return tex->getHandle();
            }

            const IPoint2D& getSize() const {
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
    using MaterialBufferData = array<GLfloat, 16>;
    struct Material {
            enum TEX_TYPE
                : GLint {
                    AMBIENT,
                DIFFUSE,
                SPECULAR,
                ALPHA,
                BUMP
            };

            GLfloat transparent = 1.f,
                    shine = 1.f;
            GLbyte illum_model = 0;
            Color col[SPECULAR + 1];

            string tex[BUMP + 1];
            string name;
            GLuint tex_array_handle = 0; // handle wspóldzielony ze wszystkimi materialami w meshu
            GLuint ubo = 0;

            Material() {
            }
            Material(const Material&);
            Material(const Color&);

            MaterialBufferData getMaterialBufferData() const;
    };

    using MATERIALS = vector<Material*>;
}

#endif
