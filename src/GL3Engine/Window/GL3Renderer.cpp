#include "Window.hpp"

namespace GL3Engine {
    void GL3Renderer::init() {
        axis = unique_ptr < Shape > (Primitives::genAxis(17));

        cam.pos[1] += 1.f;
        cam.pos[0] += 2.f;
        matrix.setCameraCoords(cam.pos, cam.target);

        model = unique_ptr < Shape
                > (MeshLoader::getInstance().loadMesh("model.obj"));
    }
    void GL3Renderer::render() {
        if (axis != nullptr)
            axis->draw(matrix, GL_LINES);
        if (model != nullptr) {
            matrix.pushTransform();
            matrix.model *= FMAT_MATH::scale( { 17.f, 17.f, 17.f });

            model->draw(matrix, GL_LINES);
            matrix.popTransform();
        }
    }
}

