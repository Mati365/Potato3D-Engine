#include "Game.hpp"

#include "../GL3Engine/Resources/Resources.hpp"
#include "../GL3Engine/Graphics/Light.hpp"

namespace Game {
    GameScreen::GameScreen(Window* _wnd)
            :
              wnd(_wnd),
              scene(_wnd->getBounds()) {
    }
    
    void GameScreen::init() {
        cam =
                &scene.createSceneNode<FPSCamera>()
                        .setPos( { 0.f, .5f, 0.f, 1.f })
                        .setTarget( { 0.f, .5f, .01f, 1.f });
        scene.createSceneNode<AxisMesh>();
        scene.createSceneNode<LightBatch>()
                .regObject(
                scene.createSceneNode<Light>()
                        .setPos( { 1.f, 0.5f, 1.f })
                        .setSpecular( { 1.f, 1.f, 1.f, 1.f }, 1.f)
                        .setDiffuse( { 1.f, 1.f, 1.f, 1.f, }, 6.f)
                        )
                .regObject(
                scene.createSceneNode<Light>()
                        .setPos( { 1.f, 0.5f, 1.f })
                        .setSpecular( { 1.f, 1.f, 1.f, 1.f }, 1.f)
                        .setDiffuse( { 1.f, 0.f, 0.f, 1.f, }, 1.f)
                        );
        scene.createSceneNode<Mesh>()
                .setShape(
                GlobalResourceManager::getInstance().getResource<Shape3D>(
                        "mesh/wall/wall.obj"))
                .getTransform()
                .mul(MatMatrix::scale( { .7f, .7f, .7f }))
                .mul(MatMatrix::translate( { -1.f, .5f, 1.f }));
        scene.createSceneNode<Mesh>()
                .setShape(
                GlobalResourceManager::getInstance().getResource<Shape3D>(
                        "mesh/floor/floor.obj"))
                .getTransform()
                .mul(MatMatrix::scale( { .3f, .3f, .3f }))
                .mul(MatMatrix::translate( { 0.f, 0.f, 0.f }));
        
        fbo =
                dynamic_cast<RenderQuad*>(
                &scene.createSceneNode<RenderQuad>()
                        .setSize(scene.getRenderResolution())
                        .setShaderParam("blur", { 0.f }, GL_FLOAT));
    }
    void GameScreen::render() {
        scene.draw();
        if (blur > 0.f)
            blur *= .98f;
        fbo->setShaderParam("blur", { blur }, GL_FLOAT);
    }
    
    void GameScreen::getKeyEvent(SDL_Keycode key) {
        scene.getKeyEvent('w');
    }
    void GameScreen::getMouseEvent(const IPoint2D& p, GLuint btn) {
        scene.getMouseEvent(p, btn);
    }
}

