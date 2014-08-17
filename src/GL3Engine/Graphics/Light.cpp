#include <algorithm>

#include "../Resources/Resources.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Mesh.hpp"

namespace GL3Engine {
    struct PointLightCam {
            GLenum face;
            unique_ptr<Camera> cam;
    };
#define DEF_FACE(type, x, y, z, _x, _y, _z) \
                    { \
                    type, unique_ptr<Camera>(new Camera( \
                                    Vec4 { x, y, z, 1.f }, \
                                    Vec4 { _x, _y, _z, 1.f })) \
                    }
    PointLightCam cube_cams[] = {
    DEF_FACE(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 1.f, 0.f, 0.f, 0.f, -1.f, 0.f),
    DEF_FACE(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, -1.f, 0.f, 0.f,0.f, -1.f, 0.f),

    DEF_FACE(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0.f, 1.f, 0.f, 0.f, 0.f, -1.f),
    DEF_FACE(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0.f, -1.f, 0.f,0.f, 0.f, 1.f),

    DEF_FACE(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0.f, 0.f, 1.f, 0.f, -1.f, 0.f),
    DEF_FACE(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0.f, 0.f, -1.f, 0.f, -1.f, 0.f),
    };

    void LightBatch::createBuffer() {
        buffer = genGLBuffer( {
                nullptr, sizeof(Light) * MAX_LIGHTS,
                GL_UNIFORM_BUFFER, 0,
                GL_STREAM_DRAW }, false);
        glBindBufferBase(GL_UNIFORM_BUFFER, BINDING_POINT, buffer);
    }
    void LightBatch::draw() {
        if (objects.empty())
            return;

        // Wysyłanie bloku informacji o pozycji dir itp
        vector<LightData> data(objects.size() - 1);
        for (auto& light : objects) {
            if (!IS_SET_FLAG(light->getType(), LightData::ENABLED))
                continue;
            // Depth mapy
            glCullFace(GL_FRONT);
            {
                light->drawShadows();
            }
            glCullFace(GL_BACK);
            data.push_back(light->getData());
        }
        objects.clear();

        GLfloat size = data.size(), data_len = data.size()
                * sizeof(LightData);
        glBindBuffer(GL_UNIFORM_BUFFER, buffer);
        {
            glBufferSubData(GL_UNIFORM_BUFFER, 0, data_len, &data[0]);
            glBufferSubData(GL_UNIFORM_BUFFER,
                    MAX_LIGHTS * sizeof(LightData),
                    sizeof(GLfloat), &size);
        }
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        
        scene->getSceneFlags()[SceneManager::SceneFlag::LIGHT_SHADER_BINDING] =
                BINDING_POINT;
    }

    // ---- Light
    void Light::draw() {
        if (parent)
            dynamic_cast<LightBatch*>(parent)->regObject(*this);
    }

    // ---- PointLight
    PointLight::PointLight()
            :
              cube( { 512, 256 }) {
        fbo.setFlags(RenderQuad::USE_DEPTH_BUFFER);
        fbo.setEffect(REQUIRE_RES(Shader, DEFAULT_SHADOW_SHADER));
        setType(LightData::ENABLED | LightData::POINT);
    }
    void PointLight::drawShadows() {
        Vec4 pos = {
                data.pos[0],
                data.pos[1],
                data.pos[2],
                1.f
        };
        for (auto& side : make_pair(cube_cams,
                cube_cams + CubeTexture::CUBE_TEX_FACES)) {
            Camera cam = {
                    pos + side.cam->getPos(),
                    pos + side.cam->getTarget()
            };
            fbo.begin();
            fbo.end();
        }
    }
}

