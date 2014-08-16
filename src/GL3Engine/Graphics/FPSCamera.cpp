#include "Camera.hpp"

namespace GL3Engine {
    GLboolean FPSCamera::getKeyEvent(GLchar key) {
        switch (key) {
            case 'w': {
                Vec3 v = MatMatrix::normalize( {
                        target[0] - pos[0],
                        0.f,
                        target[2] - pos[2] });
                v *= Vec3 { .05f, .05f, .05f };
                MatMatrix::mul( { &pos, &target }, MatMatrix::translate(v));
            }
                break;
                
            default:
                return false;
        }
        return true;
    }
    GLboolean FPSCamera::getMouseEvent(const Vec2i& p, GLuint) {
        static Vec2 last_mouse_pos = { 0.f, 0.f };
        GLfloat sensinity = .00006f;
        Vec2 v = {
                #define CHECK_SPEED(var) p.var() > 0.f ? -sensinity : p.var() < 0 ? sensinity : 0.f
                CHECK_SPEED(X),
                CHECK_SPEED(Y) };
        v[0] *= abs(p.X() - last_mouse_pos.X());
        v[1] *= abs(p.Y() - last_mouse_pos.Y());

        target -= pos;
        {
            MatMatrix::mul(target,
                    MatMatrix::rotate(
                            Tools::toRad<GLfloat>(v.X()),
                            { 0.f, 1.f, 0.f }));
        }
        target += pos;
        return true;
    }
}
