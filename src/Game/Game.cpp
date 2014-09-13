#include "Game.hpp"

#include "../GL3Engine/Resources/Resources.hpp"
#include "../GL3Engine/Graphics/Light.hpp"

namespace Game {
    NAMESPACE_IMPORT(GL3Engine::SceneObject);
    TYPE_IMPORT(GL3Engine::CoreWindow, Window);
    TYPE_IMPORT(GL3Engine::CoreMatrix, Vec2i);
    TYPE_IMPORT(GL3Engine::CoreMatrix, MatMatrix);
    TYPE_IMPORT(GL3Engine::Resources, GlobalResourceManager);

    GameScreen::GameScreen(Window* _wnd)
            :
              wnd(_wnd),
              scene(_wnd->getBounds()),
              mesh(nullptr) {
    }
    
    void GameScreen::init() {
        scene.createSceneNode<FPSCamera>()
                .setPos( { 0.f, .5f, 0.f, 1.f })
                .setTarget( { 0.f, .5f, .01f, 1.f });
        scene.createSceneNode<AxisMesh>();
        scene.createSceneNode<GL3Engine::CoreLighting::LightBatch>()
                .regObject(
                scene.createSceneNode<GL3Engine::CoreLighting::PointLight>()
                        .setPos( { 0.f, .5f, 0.f })
                        .setSpecular( { 1.f, 1.f, 1.f, 1.f }, 1.f)
                        .setDiffuse( { 1.f, 1.f, 1.f, 1.f, }, 6.f));

        mesh = &scene.createSceneNode<Mesh>()
                .setShape(
                GlobalResourceManager::getInstance().getResource<Shape3D>(
                        "mesh/wall/wall.obj"));
        mesh->getTransform()
                .mul(MatMatrix::scale( { .7f, .7f, .7f }))
                .mul(MatMatrix::translate( { 2.f, .5f, 1.f }));

        scene.createSceneNode<Mesh>()
                .setShape(
                GlobalResourceManager::getInstance().getResource<Shape3D>(
                        "mesh/wall/wall.obj"))
                .getTransform()
                .mul(MatMatrix::scale( { .7f, .7f, .7f }))
                .mul(MatMatrix::translate( { -2.f, .5f, 1.f }));

        scene.createSceneNode<Mesh>()
                .setShape(
                GlobalResourceManager::getInstance().getResource<Shape3D>(
                        "mesh/wall/wall.obj"))
                .getTransform()
                .mul(MatMatrix::scale( { .7f, .7f, .7f }))
                .mul(MatMatrix::translate( { 2.f, .5f, -3.f }));

        scene.createSceneNode<Mesh>()
                .setShape(
                GlobalResourceManager::getInstance().getResource<Shape3D>(
                        "mesh/wall/wall.obj"))
                .getTransform()
                .mul(MatMatrix::scale( { .7f, .7f, .7f }))
                .mul(MatMatrix::translate( { -2.f, .5f, -3.f }));

        scene.createSceneNode<Mesh>()
                .setShape(
                GlobalResourceManager::getInstance().getResource<Shape3D>(
                        "mesh/bob/bob_lamp_update.md5mesh"))
                .getTransform()
                .mul(MatMatrix::scale( { .03f, .03f, .03f }))
                .mul(MatMatrix::translate( { -1.f, 3.5f, 0.5f }));

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
                        .setSize(scene.getRenderResolution()));
    }
    void GameScreen::draw() {
        scene.draw();
        if (blur > 0.001f)
            blur *= .98f;
        else
            blur = 0.f;

        fbo->getEffectMgr().setEffectParam("blur", GL_FLOAT)[0] = blur;
        mesh->getTransform().mul(MatMatrix::rotate(.0001f, { 0.f, 1.f, 0.f }));
    }
    
    GLboolean GameScreen::getKeyEvent(GLchar key) {
        return scene.getKeyEvent(key);
    }
    GLboolean GameScreen::getMouseEvent(const Vec2i& p, GLuint btn) {
        blur = .015f;
        return scene.getMouseEvent(p, btn);
    }
}

