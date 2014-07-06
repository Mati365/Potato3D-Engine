#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_
#include "Types.hpp"

namespace GL3Engine {
    using namespace Tools;
    using namespace std;

    class Texture {
        private:
            GLuint handle = 0;
            IPoint2D size;

        public:
            Texture() {
            }
            Texture(const string&);

            void loadTexture(const string&);

            GLuint getHandle() const {
                return handle;
            }
            IPoint2D getSize() const {
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
    class Tile {
        private:
            Texture tex;
            IPoint2D cells;
            FPoint2D cell_size;
            Vertex* vertices; // linia po linii, brak powtórzen

        public:
            Tile(Texture, IPoint2D);

            inline IPoint2D getCells() const {
                return cells;
            }
            inline FPoint2D getCellSize() const {
                return cell_size;
            }
            inline GLuint getHandle() const {
                return tex.getHandle();
            }
            inline IPoint2D getSize() const {
                return tex.getSize();
            }
            inline Texture& getTexture() {
                return tex;
            }

            ~Tile() {
                safeDelete<Vertex>(vertices, true);
            }
    };

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

            Material() {
            }
            Material(const Material& mat) {
                name = mat.name;
                transparent = mat.transparent;
                shine = mat.shine;
                illum_model = mat.illum_model;

                memcpy(col, mat.col, sizeof(col));
                memcpy(tex, mat.tex, sizeof(tex));
            }
            Material(const Color& diffuse_color) {
                col[DIFFUSE] = diffuse_color;
            }
    };

    using MATERIALS = vector<Material*>;
}

#endif
