#include "Game.hpp"

namespace Game {
    void GameScreen::init() {
        axis = unique_ptr < Shape > (Primitives::genAxis(17));
        model = unique_ptr < Shape
                > (MeshLoader::getInstance().load<Shape>("Earth/Earth.obj"));

        matrix.selectCam(matrix.addCam(&cam));
    }
    void GameScreen::render() {
        if (axis != nullptr)
            axis->draw(matrix, GL_LINES);

        if (model != nullptr) {
            static GLfloat angle = 0.f;
            angle += 0.000005f;

            matrix.pushTransform();
            matrix.model *= FMAT_MATH::scale( { 1.1f, 1.1f, 1.1f });
            matrix.model *= FMAT_MATH::translate( { .0f, .0f, 1.05f });
            matrix.model *= FMAT_MATH::rotate(Tools::toRad<GLfloat>(angle), {
                    0.f, 1.f, 0.f });

            model->draw(matrix, GL_TRIANGLES);
            matrix.popTransform();
        }
    }
    void GameScreen::getKeyEvent(SDL_Keycode key) {
        static GLfloat angle = 0.f;
        switch (key) {
            case SDLK_LEFT:
                angle -= .1f;
                cam.target = Vec4( { 0.f, .5f, 1.f, 1.f })
                        * FMAT_MATH::rotate(angle, { 0.f,
                                1.f, 0.f });
                break;

            case SDLK_RIGHT:
                angle += .1f;
                cam.target = Vec4( { 0.f, .5f, 1.f, 1.f })
                        * FMAT_MATH::rotate(angle, { 0.f,
                                1.f, 0.f });
                break;

            case SDLK_UP:
                cam.pos += Vec4({cam.target.matrix[0] / 10, 0.f, cam.target.matrix[2] / 10, 0.f});
                break;

            case SDLK_DOWN:
                break;
        }
        matrix.updateCameraCoords();
    }
}

