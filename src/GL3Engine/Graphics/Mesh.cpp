#include "../Resources/Resources.hpp"

#include "Mesh.hpp"
#include "Light.hpp"
#include "Camera.hpp"

namespace GL3Engine {
    namespace SceneObject {
        TYPE_IMPORT(CoreEffect, Shader);
        TYPE_IMPORT(CoreEffect, UniformBufferManager);
        TYPE_IMPORT(CoreMatrix, MatMatrix);

        Mesh::Mesh(Shape3D* _shape)
                :
                  shape(_shape) {
            setEffect(REQUIRE_RES(Shader, DEFAULT_MESH_SHADER));
            setAttrib(USE_MATERIALS | USE_LIGHTING);
        }
        void Mesh::updateMaterialsCache(Shader* effect) {
            material_cache.clear();
            if (!shape->hasMaterials()
                    || !UniformBufferManager::getBlockSize(effect,
                            "MaterialBlock"))
                return;

            for (CoreMaterial::Material* mat : shape->getMaterials())
                material_cache.push_back(mat->getMaterialBufferData());

            try {
                effect->regGlobalBuffer(
                        -1,
                        GET_SCENE_FLAG(scene, MATERIAL_BUFFER_BINDING),
                        GL_STREAM_DRAW,
                        nullptr,
                        &material_cache[0],
                        "MaterialBlock");
            } catch (const GLint&) {
                material_cache.clear();
            }
        }
        
        void Mesh::passToShader() {
            if (material_cache.empty())
                updateMaterialsCache(effect);

            world->pushAttrib();
            world->attrib.model *= transform.model;
            {
                effect->bindBlockToSlot("MaterialBlock",
                        GET_SCENE_FLAG(scene, MATERIAL_BUFFER_BINDING));
                effect->bindBlockToSlot("LightBlock",
                        GET_SCENE_FLAG(scene, LIGHT_SHADER_BINDING));

                effect->setUniform("matrix.mvp",
                        world->attrib.vp_matrix * world->attrib.model)

                .setUniform("matrix.m", world->attrib.model)

                .setUniform("matrix.v", world->attrib.view)

                .setUniform("matrix.normal",
                        MatMatrix::inverse(
                                (world->attrib.view * world->attrib.model)
                                        .getCut(3, 3)))

                .setUniform("matrix.cam",
                        world->getActiveCamera()->getPos());
            }
            if (shape->getMaterials().empty())
                effect->setUniform("col", shape->getColor());
            else if (!material_cache.empty()) {
                if (IS_SET_FLAG(attrib, USE_LIGHTING))
                    effect->setUniform(GL_TEXTURE_2D_ARRAY,
                            "texture_pack", 0,
                            shape->getMaterials()[0]->tex_array->getHandle());

                if (IS_SET_FLAG(attrib, USE_MATERIALS))
                    UniformBufferManager::getInstance().changeBufferData(
                            GET_SCENE_FLAG(scene, MATERIAL_BUFFER_BINDING),
                            &material_cache[0],
                            material_cache.size()
                                    * sizeof(CoreMaterial::MaterialBufferData));
            }
            world->popAttrib();
        }
        void Mesh::draw() {
            assert(effect);
            {
                passToShader();
                glBindVertexArray(shape->getVAO());
                if (!shape->usingElementBuffer())
                    glDrawArrays(config.gl_render_flag, 0,
                            shape->getVerticesCount());
                else
                    glDrawElements(config.gl_render_flag,
                            shape->getIndicesCount(),
                            GL_UNSIGNED_SHORT, nullptr);
                glBindVertexArray(0);
            }
        }
    }
}

