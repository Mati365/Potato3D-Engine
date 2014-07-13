#include "Matrix.hpp"

namespace GL3Engine {
    MatrixStack::MatrixStack(const FPoint2D& _resolution)
            :
              resolution(_resolution) {
        switchMode(Mode::_3D);
    }
    void MatrixStack::switchMode(Mode mode) {
        switch (mode) {
            case _3D: {
                glClear(GL_DEPTH_BUFFER_BIT);
                glDepthMask(GL_TRUE);
                glDepthFunc(GL_LESS);
                glEnable(GL_DEPTH_TEST);
                {
                    projection = FMAT_MATH::perspective(45.f,
                            resolution.X / resolution.Y, 1.f,
                            100.f);
                    model = FMAT_MATH::identity();
                    updateCameraCoords();
                }
            }
                break;

            case _2D: {
                glClear(GL_DEPTH_BUFFER_BIT);
                glDepthMask(GL_FALSE);
                glDisable(GL_DEPTH_TEST);
                {
                    projection = FMAT_MATH::orthof( {
                            FPoint2D { -resolution.X / 2, resolution.X / 2 },
                            FPoint2D { -resolution.Y / 2, resolution.Y / 2 },
                            FPoint2D { 0.f, 1.f },
                    });
                    view = FMAT_MATH::identity();
                    model = view;
                    vp_matrix = projection;
                }
            }
                break;
        }
    }
    void MatrixStack::updateCameraCoords() {
        if (cam.empty() || cam.size() < active_cam)
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
