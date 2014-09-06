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
    void LightBatch::update() {
        if (objects.empty())
            return;

        // Wysyłanie bloku informacji o pozycji dir itp
        Shader* mesh_shader = REQUIRE_RES(Shader, DEFAULT_MESH_SHADER);
        vector<LightData> data(objects.size() - 1);
        for (GLuint i = 0; i < objects.size(); ++i) {
            Light* light = objects[i];
            if (!IS_SET_FLAG(light->getType(), LightData::ENABLED))
                continue;
            // Shadow mapy
            glCullFace(GL_FRONT);
            {
                light->update();
                switch (light->getHash()) {
                    case CLASS_HASH(PointLight):
                        mesh_shader->setUniform(GL_TEXTURE_CUBE_MAP,
                                "shadow_maps[" + toString(i) + "].point",
                                i + 1,
                                light->getShadowTex()->getHandle());
                        break;

                    case CLASS_HASH(DirectLight):
                        mesh_shader->setUniform(GL_TEXTURE_2D,
                                "shadow_maps[" + toString(i) + "].direct",
                                i + 1,
                                light->getShadowTex()->getHandle());
                        break;
                }
            }
            glCullFace(GL_BACK);
            data.push_back(light->getData());
        }
        //objects.clear();

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
        SET_SCENE_FLAG(scene, LIGHT_SHADER_BINDING, BINDING_POINT);
    }

    // ---- DirectLight
    DirectLight::DirectLight() {
        tex = new Texture(Vec2i { 256, 256 },
                TextureFlags { GL_RED, GL_FLOAT,
                        Texture::CLAMP_TO_EDGE | Texture::NEAREST,
                        GL_TEXTURE_2D });

        shadow_fbo.attachTex(GL_COLOR_ATTACHMENT0,
                tex,
                GL_TEXTURE_2D);
        shadow_fbo.setSize(Vec2i { 256, 256 });
        setType(LightData::ENABLED | LightData::DIRECT);
    }
    void DirectLight::update() {
        Shader* shadow_effect = REQUIRE_RES(Shader, DEFAULT_SHADOW_SHADER);
        Camera* cam = world->getActiveCamera();
        if (!cam)
            return;

        Vec4 pos = {
                cam->getPos()[0] - 5.f,
                cam->getPos()[1] - 5.f,
                cam->getPos()[2] - 5.f,
                1.f
        };
        {
            Camera cam = {
                    pos,
                    Vec4 {
                            pos[0] + data.pos[0],
                            pos[1] + data.pos[1],
                            pos[2] + data.pos[2],
                            1.f
                    }
            };
            // Shadow mapping
            world->setCam(&cam);
            shadow_fbo.begin();
            for (auto& node : *scene) {
                node->pushAttrib();
                node->setAttrib(Mesh::NONE);
                {
                    node->getEffectMgr()
                            .pushAttrib()
                            .setAttrib(shadow_effect);
                    shadow_effect->setUniform("light_pos", pos);
                    node->draw();
                    node->getEffectMgr().
                            popAttrib();
                }
                node->popAttrib();
            }
            shadow_fbo.end();
        }
        world->setCam(cam);
    }

    // ---- PointLight
    PointLight::PointLight() {
        cube = new CubeTexture( { 32, 32 },
                TextureFlags { GL_RED, GL_FLOAT,
                        Texture::CLAMP_TO_EDGE | Texture::NEAREST,
                        GL_TEXTURE_CUBE_MAP });
        shadow_fbo.attachTex(GL_COLOR_ATTACHMENT0,
                cube,
                GL_TEXTURE_CUBE_MAP_POSITIVE_X);
        shadow_fbo.setSize(cube->getSize());

        setType(LightData::ENABLED | LightData::POINT);
    }
    void PointLight::update() {
        Vec4 pos = {
                data.pos[0],
                data.pos[1],
                data.pos[2],
                1.f
        };
        Camera* cam = world->getActiveCamera();
        Shader* shadow_effect = REQUIRE_RES(Shader, DEFAULT_SHADOW_SHADER);

        for (auto& side : make_pair(cube_cams,
                cube_cams + CubeTexture::CUBE_TEX_FACES)) {
            Camera cam = {
                    pos + side.cam->getPos(),
                    pos + side.cam->getTarget()
            };
            world->setCam(&cam);
            shadow_fbo.setRenderFace(GL_COLOR_ATTACHMENT0, side.face);

            shadow_effect->begin();
            shadow_effect->setUniform("light_pos", pos);
            for (auto& node : *scene) {
                if (node->getHash() != CLASS_HASH(Mesh))
                    continue;
                node->pushAttrib();
                {
                    node->setAttrib(Mesh::NONE);
                    node->draw();
                }
                node->popAttrib();
            }
            shadow_effect->end();
        }
        world->setCam(cam);
    }
}

