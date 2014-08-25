#include "../Resources/Resources.hpp"

#include "Mesh.hpp"
#include "Light.hpp"
#include "Camera.hpp"

namespace GL3Engine {
    Mesh::Mesh(Shape3D* _shape)
            :
              shape(_shape) {
        setEffect(REQUIRE_RES(Shader, DEFAULT_MESH_SHADER));
    }
    void Mesh::updateMaterialsCache(Shader* effect) {
        material_cache.clear();
        if (!shape->hasMaterials())
            return;
        
        for (Material* mat : shape->getMaterials())
            material_cache.push_back(mat->getMaterialBufferData());
        effect->regGlobalBuffer(
                -1,
                GET_SCENE_FLAG(scene, MATERIAL_BUFFER_BINDING),
                GL_STREAM_DRAW,
                &material_cache[0],
                "MaterialBlock");
    }
    
    void Mesh::passToShader() {
        if (material_cache.empty())
            updateMaterialsCache(effect);
        
        world->pushAttrib();
        {
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
            else {
                effect->setUniform(GL_TEXTURE_2D_ARRAY,
                        "texture_pack", 0,
                        shape->getMaterials()[0]->tex_array->getHandle());
                UniformBufferManager::getInstance().changeBufferData(
                        GET_SCENE_FLAG(scene, MATERIAL_BUFFER_BINDING),
                        &material_cache[0],
                        material_cache.size() * sizeof(MaterialBufferData));
            }
        }
        world->popAttrib();
    }
    void Mesh::draw() {
        assert(effect);
        {
            passToShader();
            glBindVertexArray(shape->getVAO());
            if (!shape->usingElementBuffer())
                glDrawArrays(render_mode, 0, shape->getVerticesCount());
            else
                glDrawElements(render_mode, shape->getIndicesCount(),
                GL_UNSIGNED_SHORT, nullptr);
            glBindVertexArray(0);
        }
    }
}

