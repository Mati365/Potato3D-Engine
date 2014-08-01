#include "../Resources/Resources.hpp"

#include "Mesh.hpp"
#include "Light.hpp"

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
                LightManager::BINDING_POINT + 1);
    }

    void Mesh::passToShader(MatrixStack& matrix, Shader* effect) {
        if (!effect)
            return;
        if (material_cache.empty())
            updateMaterialsCache(effect);

        matrix.pushTransform();
        matrix.model *= transform.model;
        {
            effect->setUniform("matrix.mvp", matrix.vp_matrix * matrix.model);
            effect->setUniform("matrix.m", matrix.model);
            effect->setUniform("matrix.normal",
                    MatMatrix::inverse(matrix.model.getCut(3, 3)));
            effect->setUniform("matrix.cam", matrix.getActiveCamera()->pos);
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
    void Mesh::draw(MatrixStack& matrix, GLint mode, Shader* effect) {
        if (!effect)
            effect = REQUIRE_RES(Shader, DEFAULT_MESH_SHADER);
        effect->begin();
        {
            passToShader(matrix, effect);
            glBindVertexArray(shape->getVAO());
            if (!shape->usingElementBuffer())
                glDrawArrays(mode, 0, shape->getVerticesCount());
            else
                glDrawElements(mode, shape->getIndicesCount(),
                GL_UNSIGNED_SHORT, nullptr);
            glBindVertexArray(0);
        }
        effect->end();
    }
}

