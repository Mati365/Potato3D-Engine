#include <algorithm>

#include "Light.hpp"
#include "Mesh.hpp"

namespace GL3Engine {
    void LightBatch::createBuffer() {
        buffer = genGLBuffer( {
                nullptr,
                sizeof(Light) * MAX_LIGHTS,
                GL_UNIFORM_BUFFER,
                0,
                GL_STREAM_DRAW
        }, false);
        glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT, buffer);
    }
    void LightBatch::draw() {
        if (objects.empty())
            return;

        vector<LightData> data(objects.size() - 1);
        for (auto& obj : objects)
            if (obj->getState() == LightData::ON)
                data.push_back(obj->getData());
        objects.clear();

        GLfloat size = data.size(),
                data_len = data.size() * sizeof(LightData);
        glBindBuffer(GL_UNIFORM_BUFFER, buffer);
        {
            glBufferSubData(GL_UNIFORM_BUFFER, 0, data_len, &data[0]);
            glBufferSubData(GL_UNIFORM_BUFFER,
                    MAX_LIGHTS * sizeof(LightData) - sizeof(GLfloat),
                    sizeof(GLfloat),
                    &size);
        }
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        scene->getSceneFlags()[SceneManager::SceneFlag::LIGHT_SHADER_BINDING] =
                BINDING_POINT;
    }

    void Light::draw() {
        if (parent)
            dynamic_cast<LightBatch*>(parent)->regObject(*this);
    }
}

