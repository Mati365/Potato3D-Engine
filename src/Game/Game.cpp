#include "Game.hpp"

#include "../GL3Engine/Graphics/Text.hpp"

namespace Game {
    void GameScreen::init() {
        axis = unique_ptr < Mesh > (Primitives::genAxis(17));
        model =
                unique_ptr < Mesh
                        > (
                        new Mesh(
                                MeshLoader::getInstance().load<Shape3D>(
                                        "truck/untitled.obj"),
                                GET_SHADER(ShaderManager::DEFAULT_MESH_SHADER)));

        matrix.selectCam(matrix.addCam(&cam));

        font = new Font("sprites/font.png");
        text = new TextRenderer(font);
    }
    void GameScreen::render() {
        if (axis != nullptr)
            axis->draw(matrix, GL_LINES);

        //text.setText("truck");
        matrix.pushTransform();
        matrix.model *= FMAT_MATH::scale( { 2.0f, 0.4f, 1.4f });
        matrix.model *= FMAT_MATH::translate( { 0.0f, 0.7f, 0.5f });
        text->draw(matrix, 0);
        matrix.popTransform();

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
    }
    void GameScreen::getKeyEvent(SDL_Keycode key) {
        Vec4 v(
                {
                        0.2f,
                        0.f,
                        0.2f,
                        0.f });
        switch (key) {
            case SDLK_LEFT:
                cam.target *= FMAT_MATH::rotate(360 - 0.1, { 0.f,
                        1.f, 0.f });
                break;

            case SDLK_RIGHT:
                cam.target *= FMAT_MATH::rotate(0.1, { 0.f,
                        1.f, 0.f });
                break;

            case SDLK_UP:
                cam.pos += v;
                cam.target += v;
                break;

            case SDLK_DOWN:
                cam.pos -= v;
                cam.target -= v;
                break;
        }
        matrix.updateCameraCoords();
    }
}

