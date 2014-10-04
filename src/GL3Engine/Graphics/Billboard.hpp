#ifndef BILLBOARD_HPP_
#define BILLBOARD_HPP_
#include "Mesh.hpp"
#include "../Resources/Loaders.hpp"

namespace GL3Engine {
    namespace SceneObject {
        /** Podlega światłu, może być kilka materiałów! */
        class MaterialQuad :
                             public Shape3D {
            public:
                MaterialQuad(const CoreLoader::MaterialTextureData&);
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
    //        scene.createSceneNode<Mesh>()
    //                .setShape(
    //                new Quad3D( { "", "sprites/billboard.png", "", "", "" }))
    //                .setAttrib(Mesh::Flags::NORMAL |
    //                Mesh::Flags::USE_MATERIALS |
    //                Mesh::Flags::DISABLE_CULL_FACING)
    //                .getTransform()
    //                .mul(MatMatrix::scale( { .7f, .7f, .7f }))
    //                .mul(MatMatrix::translate( { 0.f, .5f, 2.f }));
    }
}

#endif
