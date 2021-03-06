#ifndef MATRIXSTACK_HPP_
#define MATRIXSTACK_HPP_
#include "Matrix.hpp"
#include "AttribContainer.hpp"

namespace GL3Engine {
    namespace SceneObject {
        class Camera;
    }
    namespace CoreMatrix {
        class MVPArray {
            public:
                Mat4 proj, view, model, vp_matrix;

                MVPArray() {
                }
                MVPArray(const MVPArray&);
                MVPArray& operator=(const MVPArray&);
        };
        class MatrixStack :
                            public CoreAttrib::AttribContainer<MVPArray,
                                    MatrixStack> {
            public:
                enum Mode {
                    _3D,
                    _2D
                };

            private:
                SceneObject::Camera* active_cam = nullptr;
                Vec2i resolution;

            public:
                MatrixStack(const Vec2i&);

                void switchMode(GLuint);
                void updateCameraCoords();

                MatrixStack& setCam(SceneObject::Camera*);
                SceneObject::Camera* getActiveCamera() const {
                    return active_cam;
                }
                const Vec2i& getResolution() const {
                    return resolution;
                }

                void loadModel(const Mat4&);
        };
    }
}

#endif
