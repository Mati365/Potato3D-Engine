#include "Billboard.hpp"
#include "Camera.hpp"

namespace GL3Engine {
    namespace SceneObject {
        Billboard::Billboard() {
            setEffect(
                    REQUIRE_RES(CoreEffect::Shader, DEFAULT_BILLBOARD_SHADER));
        }

        void Billboard::passToShader() {
            assert(effect && tex);
            {
                effect->setUniform("col", color)

                .setUniform(GL_TEXTURE_2D, "texture", 0, tex->getHandle())

                .setUniform("matrix.mv",
                        world->attrib.view *
                                world->attrib.model *
                                transform.model)

                .setUniform("matrix.proj", world->attrib.proj)

                .setUniform("matrix.scale",
                        CoreMatrix::Vec2 { transform.model[0], transform.model[5] });
            }
        }
        void Billboard::draw() {
            assert(effect);
            {
                passToShader();
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            }
        }
    }
}

