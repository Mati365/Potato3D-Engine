#include "Game.hpp"

#include "../GL3Engine/Graphics/Text.hpp"
#include "../GL3Engine/Resources/Resources.hpp"

namespace Game {
    void GameScreen::init() {
        axis = unique_ptr < Mesh > (Primitives::genAxis(17));
        model = new Mesh(
                GlobalResourceManager::getInstance().getResource<Shape3D>(
                        "truck/untitled.obj"),
                REQUIRE_RES(Shader, DEFAULT_MESH_SHADER));
        matrix.selectCam(matrix.addCam(&cam));
    }
    void GameScreen::render() {
        matrix.switchMode(MatrixStack::_3D);
        if (axis != nullptr)
            axis->draw(matrix, GL_LINES);

        if (model != nullptr) {
            static GLfloat angle = 0.f;
            angle += 0.000005f;

            matrix.pushTransform();
            matrix.model *= FMAT_MATH::scale( { 0.2f, 0.2f, 0.2f });
            matrix.model *= FMAT_MATH::translate( { 0.0f, 0.5f, -10.0f });

            matrix.model *= FMAT_MATH::rotate(Tools::toRad<GLfloat>(angle), {
                    0.f, 1.f, 0.f });

            model->draw(matrix, GL_TRIANGLES);
            matrix.popTransform();
        }
        Primitives::printText(matrix, "test czcionek", { 1.f, 1.f, 1.f}, 3.f, nullptr);
    }

    void GameScreen::getKeyEvent(SDL_Keycode key) {
        FPoint3D v = normalize(FPoint3D {
                cam.target.matrix[0],
                0.f,
                cam.target.matrix[2]
        });
        v *= {.05f, .05f, .05f};

        Mat4 transform = FMAT_MATH::translate(v);
        transform.print();

        FMAT_MATH::mul(cam.pos, transform);
        FMAT_MATH::mul(cam.target, transform);
    }
    void GameScreen::getMouseEvent(const IPoint2D& p, GLuint btn) {
        static IPoint2D last_mouse_pos = { 0, 0 };

        GLfloat sensinity = .00006f,
                v = (GLfloat) p.X > 0.f ? -sensinity :
                                          (p.X < 0 ? sensinity : 0.f);
        v *= abs(p.X - last_mouse_pos.X);
        cam.target =
                FMAT_MATH::rotate(
                        Tools::toRad<GLfloat>(v),
                        { 0.f, 1.f, 0.f }) * cam.target;
        last_mouse_pos = p;
    }
}

