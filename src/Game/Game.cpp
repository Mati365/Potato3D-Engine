#include "Game.hpp"

#include "../GL3Engine/Graphics/Text.hpp"
#include "../GL3Engine/Resources/Resources.hpp"
#include "../GL3Engine/Graphics/Light.hpp"
#include "../GL3Engine/Graphics/RenderTarget.hpp"

namespace Game {
    GameScreen::GameScreen(Window* _wnd)
            :
              wnd(_wnd),
              scene(_wnd->getBounds()) {
    }

    void GameScreen::init() {
        cam = &scene.createSceneNode<Camera>()
                .setPos( { 0.f, .5f, 1.f, 1.f })
                .setTarget( { 0.f, .5f, -2.f, 1.f });
        scene.createSceneNode<AxisMesh>();
        scene.createSceneNode<LightBatch>()
                .regObject(
                scene.createSceneNode<Light>()
                        .setPos( { 0.f, 0.5, 0.f })
                        .setSpecular( { 1.f, 1.f, 1.f, 1.f }, 1.f)
                        .setDiffuse( { 1.f, 1.f, 1.f, 1.f, }, 1.f)
                        );
        scene.createSceneNode<Mesh>()
                .setShape(
                GlobalResourceManager::getInstance().getResource<Shape3D>(
                        "mesh/truck/untitled.obj"))
                .getTransform()
                .mul(MatMatrix::scale( { .2f, .2f, .2f }))
                .mul(MatMatrix::translate( { 0.f, 0.5f, -10.f }));

        scene.createSceneNode<Text>()
                .setText("Test czcionek!")
                .setPos( { 0.f, 0.5f, 0.f })
                .setSize(4.f);

        //RenderQuad* fbo = new RenderQuad({ 420, 320});
        //scene.addSceneNode(fbo);
        scene.createSceneNode<RenderQuad>()
                .setSize(scene.getRenderResolution());
    }
    void GameScreen::render() {
        scene.draw();
    }

    void GameScreen::getKeyEvent(SDL_Keycode key) {
        assert(cam);
        FPoint3D v = normalize(FPoint3D {
                cam->getTarget().matrix[0],
                0.f,
                cam->getTarget().matrix[2]
        });
        v *= {.05f, .05f, .05f};
        {
            Mat4 transform = MatMatrix::translate(v);
            MatMatrix::mul(cam->getPos(), transform);
            MatMatrix::mul(cam->getTarget(), transform);
        }
    }
    void GameScreen::getMouseEvent(const IPoint2D& p, GLuint btn) {
        static IPoint2D last_mouse_pos = { 0, 0 };
        GLfloat sensinity = .00006f,
                v = (GLfloat) p.X > 0.f ? -sensinity :
                                          (p.X < 0 ? sensinity : 0.f);
        v *= abs(p.X - last_mouse_pos.X);
        assert(cam);
        {
            MatMatrix::mul(
                    cam->getTarget(),
                    MatMatrix::rotate(
                            Tools::toRad<GLfloat>(v),
                            { 0.f, 1.f, 0.f }));
            last_mouse_pos = p;
        }
    }
}

