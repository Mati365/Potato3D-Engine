#include "Billboard.hpp"

namespace GL3Engine {
    namespace SceneObject {
        TYPE_IMPORT(CoreType, Vertex4f);
        TYPE_IMPORT(std, vector);

        vector<Vertex4f> vertex_array = {
                {
                        /** Vert */-.5f, -.5f, 0.f,
                        /** N */0.f, 0.f, -1.f,
                        /** Tex */0.f, 0.f,
                        /** MTL */0
                },
                {
                        /** Vert */.5f, -.5f, 0.f,
                        /** N */0.f, 0.f, -1.f,
                        /** Tex */1.f, 0.f,
                        /** MTL */0
                },
                {
                        /** Vert */.5f, .5f, 0.f,
                        /** N */0.f, 0.f, -1.f,
                        /** Tex */1.f, 1.f,
                        /** MTL */0
                },
                {
                        /** Vert */-.5f, .5f, 0.f,
                        /** N */0.f, 0.f, -1.f,
                        /** Tex */0.f, 1.f,
                        /** MTL */0
                }
        };
        Billboard::Billboard(c_str tex)
                :
                  Shape3D(
                          {
                                  &vertex_array[0],
                                  vertex_array.size() * sizeof(Vertex4f),
                                  GL_ARRAY_BUFFER, 0,
                                  GL_STATIC_DRAW
                          },
                          {
                                  CoreMaterial::Tile::quad_indices,
                                  6 * sizeof(GLfloat),
                                  GL_ELEMENT_ARRAY_BUFFER, 0,
                                  GL_STATIC_DRAW
                          }) {
            TYPE_IMPORT(CoreMaterial, Material);
            TYPE_IMPORT(CoreMaterial, TextureArray);

            TextureArray* array = new TextureArray( { tex });
            Material* material = new Material;
            material->tex_flags = 1 << Material::DIFFUSE;
            material->tex_array.reset(array);

            setMaterials( { material });
        }
    }
}

