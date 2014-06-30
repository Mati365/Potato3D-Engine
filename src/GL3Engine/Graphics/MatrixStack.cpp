#include "Matrix.hpp"

namespace GL3Engine {
    MatrixStack::MatrixStack() {
        projection = FMAT_MATH::perspective(45.f, 1.f / 1.f, 1.f,
                100.f);
        model = FMAT_MATH::identity();
        updateCameraCoords();
    }
    void MatrixStack::updateCameraCoords() {
        view = FMAT_MATH::lookAt(
                cam.pos,
                cam.target,
                { 0, 1, 0 });
        vp_matrix = projection * view;
    }

    void MatrixStack::pushTransform() {
        M_STACK_ARRAY array;
        memcpy(array.array, model.matrix, 16 * sizeof(GLfloat));
        stack.push_back(array);
    }
    void MatrixStack::popTransform() {
        if (stack.empty())
            return;
        memcpy(model.matrix, stack.back().array, 16 * sizeof(GLfloat));
        stack.pop_back();
    }
}