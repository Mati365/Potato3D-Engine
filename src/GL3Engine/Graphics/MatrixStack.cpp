#include "Matrix.hpp"

namespace GL3Engine {
    MatrixStack::MatrixStack() {
        projection = FMAT_MATH::perspective(45.f, 480.f / 320.f, 1.f,
                100.f);
        model = FMAT_MATH::identity();
        updateCameraCoords();
    }
    void MatrixStack::updateCameraCoords() {
        if(cam.empty() || cam.size() < active_cam)
            return;
        view = FMAT_MATH::lookAt(
                cam[active_cam]->pos,
                cam[active_cam]->target,
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
