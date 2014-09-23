#ifndef BILLBOARD_HPP_
#define BILLBOARD_HPP_
#include "Mesh.hpp"
#include "../Resources/Loaders.hpp"

namespace GL3Engine {
    namespace SceneObject {
        /** Podlega światłu, może być kilka materiałów! */
        class Quad3D :
                       public Shape3D {
            public:
                Quad3D(const CoreLoader::MaterialTextureData&);
        };
        /** Brak tesktury */
        class Quad2D :
                       public Shape2D {
            public:
                Quad2D();
        };

        /**
         * Billboard, nie obowiązuje go oświetlenie,
         * szybszy niż FlatShape
         */
        class Billboard :
                          public CoreRenderer::Node {
            DECLARE_NODE_TYPE(Billboard)

            private:
                CoreMaterial::Texture* tex = nullptr;
                CoreType::Color color = { 1.f, 1.f, 1.f, 1.f };
                Quad2D quad;

            public:
                Billboard();

                void draw() override;
                Billboard& setTexture(CoreMaterial::Texture* tex) {
                    this->tex = tex;
                    return *this;
                }
                Billboard& setColor(const CoreType::Color& color) {
                    this->color = color;
                    return *this;
                }

            private:
                void passToShader() override;
        };
    }
}

#endif
