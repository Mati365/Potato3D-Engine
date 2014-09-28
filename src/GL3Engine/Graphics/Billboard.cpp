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
                glDisable(GL_CULL_FACE);
                {
                    glBindVertexArray(quad.getVAO());
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
                    glBindVertexArray(0);
                }
                glEnable(GL_CULL_FACE);
            }
        }
    }
}

