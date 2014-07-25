#include <algorithm>

#include "Light.hpp"
#include "Mesh.hpp"

namespace GL3Engine {
    void LightManager::create() {
        buffer = genGLBuffer( {
                nullptr,
                sizeof(Light) * MAX_LIGHTS,
                GL_UNIFORM_BUFFER,
                0,
                GL_STREAM_DRAW
        }, false);
        glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT, buffer);
    }
    void LightManager::update() {
        vector<Light> data(lights.size());
        copy_if(lights.begin(), lights.end(), data.begin(),
                [](Light& el) {
                    return el.state == Light::ON;
                });
        // Ostatni blok to ilość świateł
        if (data.empty())
            return;
        GLfloat size = data.size(),
                data_len = data.size() * sizeof(Light);

        glBindBuffer(GL_UNIFORM_BUFFER, buffer);
        {
            glBufferSubData(GL_UNIFORM_BUFFER, 0, data_len, &data[0]);
            glBufferSubData(GL_UNIFORM_BUFFER,
                    MAX_LIGHTS * sizeof(Light) - sizeof(GLfloat),
                    sizeof(GLfloat),
                    &size);
        }
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}

