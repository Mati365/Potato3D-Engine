#ifndef CAMERA_HPP_
#define CAMERA_HPP_
#include "Mesh.hpp"

namespace GL3Engine {
    class Camera : public Node {
        private:
            Vec4 pos;
            Vec4 target;

        public:
            Camera() {
            }
            Camera(const Vec4& _pos,
                   const Vec4& _target)
                    :
                      pos(_pos),
                      target(_target) {
            }

#define SET_VEC4_VALUE(func_name, variable) \
                inline Camera& set##func_name(array<GLfloat, 4> variable) { \
                    memcpy(this->variable.matrix, &variable[0], sizeof(GLfloat) * 4); \
                    return *this; \
                } \
                inline Vec4& get##func_name() { \
                    return variable; \
                }
            SET_VEC4_VALUE(Pos, pos)
            SET_VEC4_VALUE(Target, target)

            void draw() {
                scene->getWorldMatrix().setCam(this);
            }
    };
}

#endif
