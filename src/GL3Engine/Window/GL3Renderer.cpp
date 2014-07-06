#include "Window.hpp"

namespace GL3Engine {
    void GL3Renderer::init() {
        axis = unique_ptr < Shape > (Primitives::genAxis(17));
        model = unique_ptr < Shape
                > (MeshLoader::getInstance().load<Shape>("truck/untitled.obj"));
    }
    void GL3Renderer::render() {
        if (axis != nullptr)
            axis->draw(matrix, GL_LINES);

        if (model != nullptr) {
            static GLfloat angle = 0.f;
            angle += 0.000005f;

            matrix.pushTransform();
            matrix.model *= FMAT_MATH::scale( { .25f, .25f, .25f });
            matrix.model *= FMAT_MATH::rotate(Tools::toRad<GLfloat>(angle), {0.f, 1.f, 0.f });

            model->draw(matrix, GL_TRIANGLES);
            matrix.popTransform();
        }
    }
}

