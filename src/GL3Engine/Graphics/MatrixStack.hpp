#ifndef MATRIXSTACK_HPP_
#define MATRIXSTACK_HPP_
#include "Matrix.hpp"
#include "AttribContainer.hpp"

namespace GL3Engine {
    class Camera;
    class MVPArray {
        public:
            Mat4 proj, view, model, vp_matrix;

            MVPArray() {
            }
            MVPArray(const MVPArray&);
            MVPArray& operator=(const MVPArray&);
    };
    class MatrixStack :
                        public AttribContainer<MVPArray> {
        public:
            enum class Mode {
                _3D,
                _2D
            };

        private:
            Camera* active_cam = nullptr;
            Vec2i resolution;

        public:
            MatrixStack(const Vec2i&);

            void switchMode(Mode);
            void updateCameraCoords();

            MatrixStack& setCam(Camera*);
            Camera* getActiveCamera() const {
                return active_cam;
            }
            const Vec2i& getResolution() const {
                return resolution;
            }
            
            void loadModel(const Mat4&);
    };
}

#endif
