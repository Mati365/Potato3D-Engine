#include "Mesh.hpp"

namespace GL3Engine {
    Mesh::Mesh(Shape3D* _shape, Shader* _effect)
            :
              shape(shared_ptr < Shape3D > (_shape)),
              effect(_effect) {
        updateMaterialsCache();
    }

    void Mesh::updateMaterialsCache() {
        vector<GLfloat> data;
        if (!shape->hasMaterials()) {
            material_cache = 0;
            return;
        }
        for (Material* mat : shape->getMaterials()) {
            MaterialBufferData mat_data = mat->getMaterialBufferData();
            data.insert(data.end(), mat_data.begin(), mat_data.end());
        }
        effect->setUBO("MaterialBlock", material_cache, data, GL_STATIC_DRAW,
                1);
    }
    void Mesh::draw(MatrixStack& matrix, GLint mode) {
        if (!effect || !shape)
            return;

        effect->begin();
        effect->setUniform("matrix.mvp", matrix.vp_matrix * matrix.model);
        effect->setUniform("matrix.m", matrix.model);
        effect->setUniform("matrix.normal",
                FMAT_MATH::inverse(matrix.model.getCut(3, 3)));
        effect->setUniform("matrix.cam", matrix.getActiveCamera()->pos);
        if (shape->getMaterials().empty())
            effect->setUniform("col", shape->getColor());
        {
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

