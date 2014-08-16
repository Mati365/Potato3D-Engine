#include "MatrixStack.hpp"
#include "Camera.hpp"

namespace GL3Engine {
    MatrixStack::MatrixStack(const Vec2i& _resolution)
            :
              resolution(_resolution) {
        switchMode(Mode::_3D);
    }
    
    void MatrixStack::switchMode(Mode mode) {
        switch (mode) {
            case Mode::_3D: {
                glClear(GL_DEPTH_BUFFER_BIT);
                glDepthMask(GL_TRUE);
                glDepthFunc(GL_LESS);
                glEnable(GL_DEPTH_TEST);
                {
                    projection = MatMatrix::perspective(45.f,
                            resolution[0] / resolution[1], .1f, 100.f);
                    MatMatrix::identity(1, &model);
                    updateCameraCoords();
                }
            }
                break;
                
            case Mode::_2D: {
                glClear(GL_DEPTH_BUFFER_BIT);
                glDepthMask(GL_FALSE);
                glDisable(GL_DEPTH_TEST);
                {
                    projection = MatMatrix::orthof( {
                            Vec2 { -resolution[0] / 2.f, resolution[0] / 2.f },
                            Vec2 { -resolution[1] / 2.f, resolution[1] / 2.f },
                            Vec2 { 0.f, 1.f },
                    });
                    vp_matrix = projection;
                    MatMatrix::identity(2, &view, &model);
                }
            }
                break;
        }
    }
    void MatrixStack::updateCameraCoords() {
        if (!active_cam)
            return;
        view = MatMatrix::lookAt(active_cam->getPos(), active_cam->getTarget(),
                { 0, 1, 0 });
        vp_matrix = projection * view;
    }
    MatrixStack& MatrixStack::setCam(Camera* active_cam) {
        this->active_cam = active_cam;
        updateCameraCoords();
        return *this;

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
    void MatrixStack::loadMatrix(const Mat4& mem) {
        memcpy(model.matrix, mem.matrix, 16 * sizeof(GLfloat));
    }
}
