#include "MatrixStack.hpp"
#include "Camera.hpp"

namespace GL3Engine {
    MVPArray::MVPArray(const MVPArray& arr) {
        *this = arr;
    }
    MVPArray& MVPArray::operator=(const MVPArray& arr) {
#define MEMCPY_PUSH(variable) \
        memcpy(variable.matrix, arr.variable.matrix, 16 * sizeof(GLfloat))
        {
            MEMCPY_PUSH(proj);
            MEMCPY_PUSH(view);
            MEMCPY_PUSH(model);
            MEMCPY_PUSH(vp_matrix);
        }
        return *this;
    }

    MatrixStack::MatrixStack(const Vec2i& _resolution)
            :
              resolution(_resolution) {
        switchMode(Mode::_3D);
    }
    
    void MatrixStack::switchMode(GLuint mode) {
        switch (mode) {
            case Mode::_3D: {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glDepthMask(GL_TRUE);
                glDepthFunc(GL_LESS);
                glEnable(GL_DEPTH_TEST);
                {
                    attrib.proj = MatMatrix::perspective(45.f,
                            resolution[0] / resolution[1], .1f, 100.f);
                    MatMatrix::identity(1, &attrib.model);
                    updateCameraCoords();
                }
            }
                break;
                
            case Mode::_2D: {
                glClear(GL_COLOR_BUFFER_BIT);
                glDepthMask(GL_FALSE);
                glDisable(GL_DEPTH_TEST);
                {
                    attrib.proj = MatMatrix::orthof( {
                            Vec2 { -resolution[0] / 2.f, resolution[0] / 2.f },
                            Vec2 { -resolution[1] / 2.f, resolution[1] / 2.f },
                            Vec2 { 0.f, 1.f },
                    });
                    attrib.vp_matrix = attrib.proj;
                    MatMatrix::identity(2, &attrib.view, &attrib.model);
                }
            }
                break;
        }
    }
    void MatrixStack::updateCameraCoords() {
        if (!active_cam)
            return;
        attrib.view = MatMatrix::lookAt(active_cam->getPos(),
                active_cam->getTarget(),
                { 0.f, 1.f, 0.f });
        attrib.vp_matrix = attrib.proj * attrib.view;
    }
    MatrixStack& MatrixStack::setCam(Camera* active_cam) {
        this->active_cam = active_cam;
        updateCameraCoords();
        return *this;

    }

    void MatrixStack::loadModel(const Mat4& mem) {
        memcpy(attrib.model.matrix, mem.matrix, 16 * sizeof(GLfloat));
    }
}
