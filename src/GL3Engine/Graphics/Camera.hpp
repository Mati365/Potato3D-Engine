#ifndef CAMERA_HPP_
#define CAMERA_HPP_
#include "Mesh.hpp"

namespace GL3Engine {
    class Camera :
                   public Node {
        public:
            static constexpr GLfloat VIEW_DISTANCE = 6.f;

        protected:
            Vec4 pos;
            Vec4 target;

        public:
            Camera() {
            }
            Camera(const Vec4& _pos, const Vec4& _target)
                    :
                      pos(_pos),
                      target(_target) {
            }
            
#define SET_VEC4_VALUE(func_name, variable) \
                Camera& set##func_name(array<GLfloat, 4> variable) { \
                    memcpy(this->variable.matrix, &variable[0], sizeof(GLfloat) * 4); \
                    return *this; \
                } \
                Vec4& get##func_name() { \
                    return variable; \
                }
            SET_VEC4_VALUE(Pos, pos)
            SET_VEC4_VALUE(Target, target)

            void draw() {
                scene->getWorldMatrix().setCam(this);
            }
    };
    class FPSCamera :
                      public Camera {
        public:
            enum {
                NONE = 1 << 0,
                BLOCK_Y_AXIS = 1 << 1,
                BLOCK_X_AXIS = 1 << 2,
                INVERT_Y = 1 << 3
            };

        private:
            GLuint flags = NONE;

        public:
            FPSCamera() {
            }
            FPSCamera(const Vec4& _pos, const Vec4& _target)
                    :
                      Camera(_pos, _target) {
            }
            
            GLboolean getMouseEvent(const IPoint2D&, GLuint);
            GLboolean getKeyEvent(GLchar);

            GLuint getFlags() const {
                return flags;
            }
            FPSCamera& setFlags(GLuint flags) {
                this->flags = flags;
                return *this;
            }
    };
}

#endif
