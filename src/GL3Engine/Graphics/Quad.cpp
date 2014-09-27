#include "Billboard.hpp"

namespace GL3Engine {
    namespace SceneObject {
        TYPE_IMPORT(CoreType, Vertex4f);
        TYPE_IMPORT(CoreType, Vertex2f);

        CoreType::VertexArray quad3d_vert_array = {
                {
                        /** Vert */-.5f, -.5f, 0.f,
                        /** N */0.f, 0.f, 1.f,
                        /** Tex */0.f, 0.f,
                        /** MTL */0
                },
                {
                        /** Vert */.5f, -.5f, 0.f,
                        /** N */0.f, 0.f, 1.f,
                        /** Tex */1.f, 0.f,
                        /** MTL */0
                },
                {
                        /** Vert */.5f, .5f, 0.f,
                        /** N */0.f, 0.f, 1.f,
                        /** Tex */1.f, 1.f,
                        /** MTL */0
                },
                {
                        /** Vert */-.5f, .5f, 0.f,
                        /** N */0.f, 0.f, 1.f,
                        /** Tex */0.f, 1.f,
                        /** MTL */0
                }
        };
        std::vector<Vertex2f> quad2d_vert_array = {
                {
                        /** POS */-1.f, -1.f, 0.f,
                        /** Tex */0.f, 0.f
                },
                {
                        /** POS */1.f, -1.f, 0.f,
                        /** Tex */1.f, 0.f
                },
                {
                        /** POS */1.f, 1.f, 0.f,
                        /** Tex */1.f, 1.f
                },
                {
                        /** POS */-1.f, 1.f, 0.f,
                        /** Tex */0.f, 1.f
                },
        };
        MaterialQuad::MaterialQuad(const CoreLoader::MaterialTextureData& tex)
                :
                  Shape3D(
                          {
                                  &quad3d_vert_array[0],
                                  quad3d_vert_array.size() * sizeof(Vertex4f),
                                  GL_ARRAY_BUFFER, 0,
                                  GL_STATIC_DRAW
                          },
                          {
                                  CoreMaterial::Tile::quad_indices,
                                  6 * sizeof(GLushort),
                                  GL_ELEMENT_ARRAY_BUFFER, 0,
                                  GL_STATIC_DRAW
                          }) {
            TYPE_IMPORT(CoreMaterial, Material);
            TYPE_IMPORT(CoreMaterial, TextureArray);

            Material* material = new Material;
            material->tex_flags = OBJ::MTLloader::genTextureFlags(tex)
                    | 1 << Material::BILLBOARD;
            material->tex_array.reset(
                    new TextureArray(
                            { tex[0], tex[1], tex[2], tex[3], tex[4] }));
            material->col[Material::DIFFUSE] = {.5f, .5f, .5f, 1.f};

            setMaterials( { material });
        }
        Quad::Quad()
                :
                  Shape2D(
                          {
                                  &quad2d_vert_array[0],
                                  quad2d_vert_array.size() * sizeof(Vertex2f),
                                  GL_ARRAY_BUFFER, 0,
                                  GL_STATIC_DRAW
                          },
                          {
                                  CoreMaterial::Tile::quad_indices,
                                  6 * sizeof(GLushort),
                                  GL_ELEMENT_ARRAY_BUFFER, 0,
                                  GL_STATIC_DRAW
                          }) {
        }
    }
}
