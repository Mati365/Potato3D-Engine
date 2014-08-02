#include "../Resources/Resources.hpp"

#include "Mesh.hpp"
#include "Light.hpp"
#include "Camera.hpp"

namespace GL3Engine {
    Mesh::Mesh(Shape3D* _shape)
            :
              shape(_shape) {
    }
    void Mesh::updateMaterialsCache(Shader* effect) {
        material_cache.clear();
        if (!shape->hasMaterials())
            return;

        for (Material* mat : shape->getMaterials())
            material_cache.push_back(mat->getMaterialBufferData());
        ubo_handle = effect->setUBO("MaterialBlock", nullptr, GL_DYNAMIC_DRAW,
                1);
    }

    void Mesh::passToShader() {
        assert(effect);

        MatrixStack& matrix = scene->getWorldMatrix();
        if (material_cache.empty())
            updateMaterialsCache(effect);

        matrix.pushTransform();
        {
            matrix.model *= transform.model;

            effect->bindToSlot("LightBlock",
                    scene->getSceneFlags()[SceneManager::SceneFlag::LIGHT_SHADER_BINDING]);
            effect->setUniform("matrix.mvp", matrix.vp_matrix * matrix.model);
            effect->setUniform("matrix.m", matrix.model);
            effect->setUniform("matrix.normal",
                    MatMatrix::inverse(matrix.model.getCut(3, 3)));
            effect->setUniform("matrix.cam",
                    matrix.getActiveCamera()->getPos());
            {
                if (shape->getMaterials().empty())
                    effect->setUniform("col", shape->getColor());
                else {
                    effect->setUniform(GL_TEXTURE_2D_ARRAY, "texture_pack", 0,
                            shape->getMaterials()[0]->tex_array->getHandle());
                    effect->changeUBOData(ubo_handle, &material_cache[0],
                            material_cache.size() * sizeof(MaterialBufferData));
                }
            }
        }
        matrix.popTransform();
    }
    void Mesh::draw() {
        if (!effect)
            effect = REQUIRE_RES(Shader, DEFAULT_MESH_SHADER);
        effect->begin();
        {
            passToShader();
            glBindVertexArray(shape->getVAO());
            if (!shape->usingElementBuffer())
                glDrawArrays(
                        render_mode,
                        0,
                        shape->getVerticesCount());
            else
                glDrawElements(
                        render_mode,
                        shape->getIndicesCount(),
                        GL_UNSIGNED_SHORT,
                        nullptr);
            glBindVertexArray(0);
        }
        effect->end();
    }
}

