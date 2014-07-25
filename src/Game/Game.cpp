#include "Game.hpp"

#include "../GL3Engine/Graphics/Text.hpp"
#include "../GL3Engine/Resources/Resources.hpp"
#include "../GL3Engine/Graphics/Light.hpp"

namespace Game {
    GameScreen::GameScreen(Window* _wnd)
            :
              wnd(_wnd),
              matrix(_wnd->getBounds()) {
    }

    void GameScreen::init() {
        axis = unique_ptr < Mesh > (Primitives::genAxis(17));
        model = new Mesh(
                GlobalResourceManager::getInstance().getResource<Shape3D>(
                        "mesh/truck/untitled.obj"));
        matrix.selectCam(matrix.addCam(&cam));

        fbo = new FBO(static_cast<IPoint2D>(matrix.getResolution()));

        LightManager::getInstance().addLight(
                Light {
            0.0, 0.5, 0.0, 0.0, // Pos
            1.0, 1.0, 1.0, 1.0, // Specular col
            0.0, 0.0, 1.0, 1.0, // Diffuse col

            1.0,
            2.0,
            1.0,
            Light::ON
                });

        LightManager::getInstance().addLight(
                        Light {
                    1.0, 1.5, 0.5, 0.0, // Pos
                    1.0, 1.0, 1.0, 1.0, // Specular col
                    1.0, 0.0, 0.0, 1.0, // Diffuse col

                    1.0,
                    2.0,
                    1.0,
                    Light::ON
                        });
    }
    void GameScreen::render() {
        static Shader* mesh_shader = REQUIRE_RES(Shader, DEFAULT_MESH_SHADER);
        static Shader* fbo_shader = REQUIRE_RES(Shader, DEFAULT_FBO_SHADER);

        LightManager::getInstance().update();
        mesh_shader->bindToSlot("LightBlock", LightManager::BINDING_POINT);

        fbo->begin();
        mesh_shader->begin();
        matrix.switchMode(MatrixStack::_3D);

        if (!box)
            box =
                    new Mesh(
                            GlobalResourceManager::getInstance().getResource<
                                    Shape3D>(
                                    "mesh/wall/wall.obj"));

        if (axis)
            axis->draw(matrix, GL_LINES, mesh_shader);

        static GLfloat angle = 0.f;
        angle += 0.000005f;
        if (model) {
            matrix.pushTransform();
            matrix.model *= FMAT_MATH::scale( { 0.2f, 0.2f, 0.2f });
            matrix.model *= FMAT_MATH::translate( { 0.0f, 0.5f, -10.0f });

            matrix.model *= FMAT_MATH::rotate(Tools::toRad<GLfloat>(angle), {
                    0.f, 1.f, 0.f });

            model->draw(matrix, GL_TRIANGLES, mesh_shader);
            matrix.popTransform();
        }
        if (box) {
            matrix.pushTransform();
            matrix.model *= FMAT_MATH::translate( { 0.0f, 0.0f, 2.0f });
            matrix.model *= FMAT_MATH::scale( { 1.3f, 1.3f, 1.3f });
            matrix.model *= FMAT_MATH::rotate(Tools::toRad<GLfloat>(angle), {
                    0.f, 1.f, 0.f });

            box->draw(matrix, GL_TRIANGLES, mesh_shader);
            matrix.popTransform();
        }
        glUseProgram(0);
        fbo->end();

        fbo_shader->begin();
        fbo->draw(matrix, 0, fbo_shader);
    }

    void GameScreen::getKeyEvent(SDL_Keycode key) {
        FPoint3D v = normalize(FPoint3D {
                cam.target.matrix[0],
                0.f,
                cam.target.matrix[2]
        });
        v *= {.05f, .05f, .05f};

        Mat4 transform = FMAT_MATH::translate(v);

        FMAT_MATH::mul(cam.pos, transform);
        FMAT_MATH::mul(cam.target, transform);
    }
    void GameScreen::getMouseEvent(const IPoint2D& p, GLuint btn) {
        static IPoint2D last_mouse_pos = { 0, 0 };

        GLfloat sensinity = .00006f,
                v = (GLfloat) p.X > 0.f ? -sensinity :
                                          (p.X < 0 ? sensinity : 0.f);
        v *= abs(p.X - last_mouse_pos.X);
        FMAT_MATH::mul(
                cam.target,
                FMAT_MATH::rotate(
                        Tools::toRad<GLfloat>(v),
                        { 0.f, 1.f, 0.f }));
        last_mouse_pos = p;
    }
}

