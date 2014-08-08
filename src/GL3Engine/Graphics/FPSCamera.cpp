#include "Camera.hpp"

namespace GL3Engine {
    GLboolean FPSCamera::getKeyEvent(GLchar key) {
        switch (key) {
            case 'w': {
                FPoint3D v = normalize(FPoint3D {
                        target.matrix[0], 0.f, target.matrix[2] });
                v *= {.05f, .05f, .05f};
                {
                    Mat4 transform = MatMatrix::translate(v);
                    MatMatrix::mul( {
                            &pos, &target }, transform);
                }
            }
                break;
                
            default:
                return false;
        }
        return true;
    }
    GLboolean FPSCamera::getMouseEvent(const IPoint2D& p, GLuint) {
        static IPoint2D last_mouse_pos = {
                0, 0 };
        GLfloat sensinity = .00006f;
        FPoint2D v = {
                #define CHECK_SPEED(var) p.var > 0.f ? -sensinity : p.var < 0 ? sensinity : 0.f
                CHECK_SPEED(X), CHECK_SPEED(Y) };
        v *= IPoint2D {
                abs(p.X - last_mouse_pos.X), -abs(p.Y - last_mouse_pos.Y) };
        {
#define ROTATE_CAM(var, ...) \
            MatMatrix::mul(target, MatMatrix::rotate(Tools::toRad<GLfloat>(v.var), { __VA_ARGS__ }));
            
            ROTATE_CAM(X, 0.f, 1.f, 0.f);
            // ROTATE_CAM(Y, 1.f, 0.f, 0.f);
            last_mouse_pos = p;
        }
        return true;
    }
}
