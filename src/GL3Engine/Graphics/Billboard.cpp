#include "Billboard.hpp"

namespace GL3Engine {
    namespace SceneObject {
        Billboard::Billboard() {
            setEffect(REQUIRE_RES(CoreEffect::Shader, DEFAULT_TEXT_SHADER));
        }

        void Billboard::passToShader() {
            assert(effect && tex);
            {
                effect->setUniform("col", color)

                .setUniform(GL_TEXTURE_2D, "texture", 0, tex->getHandle())

                .setUniform("matrix.mvp",
                        world->attrib.vp_matrix *
                                world->attrib.model *
                                transform.model);
            }
        }
        void Billboard::draw() {
            assert(effect);
            {
                passToShader();
                glDisable(GL_CULL_FACE);
                {
                    glBindVertexArray(quad.getVAO());
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
                    glBindVertexArray(0);
                }
                glEnable(GL_CULL_FACE);
            }
        }
    }
}

