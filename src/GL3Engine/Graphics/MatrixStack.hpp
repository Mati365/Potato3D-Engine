#ifndef MATRIXSTACK_HPP_
#define MATRIXSTACK_HPP_
#include "Matrix.hpp"

namespace GL3Engine {
    class Camera;
    class MatrixStack {
        public:
            enum class Mode {
                _3D,
                _2D
            };
            struct M_STACK_ARRAY {
                    GLfloat array[16];
            };

        private:
            Camera* active_cam = nullptr;
            list<M_STACK_ARRAY> stack; // pushTransform i popTransform
            FPoint2D resolution;

        public:
            Mat4 projection, view, model, vp_matrix; // cache z mnożenia view * projection
                    
            MatrixStack(const FPoint2D&);

            void switchMode(Mode);
            void updateCameraCoords();

            MatrixStack& setCam(Camera*);
            Camera* getActiveCamera() const {
                return active_cam;
            }
            const FPoint2D& getResolution() const {
                return resolution;
            }
            
            void pushTransform();
            void popTransform();
            void loadMatrix(const Mat4&);
    };
}

#endif
