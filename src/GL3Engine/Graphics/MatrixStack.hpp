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
                    using MatArray = GLfloat[16];
                    MatArray projection, view, model;
            };

        private:
            Camera* active_cam = nullptr;
            list<M_STACK_ARRAY> stack; // pushTransform i popTransform
            Vec2i resolution;

        public:
            Mat4 projection, view, model, vp_matrix; // cache z mnożenia view * projection
                    
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
            
            void pushTransform();
            void popTransform();
            void loadMatrix(const Mat4&);
    };
}

#endif
