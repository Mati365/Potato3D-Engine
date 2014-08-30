#include <algorithm>

#include "../Resources/Resources.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Mesh.hpp"

namespace GL3Engine {
    GLuint m_fbo, m_shadowMap, m_depth;

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
                                m_shadowMap);
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
    void initP() {
        // Create the FBO
        glGenFramebuffers(1, &m_fbo);

        // Create the depth buffer
        glGenTextures(1, &m_depth);
        glBindTexture(GL_TEXTURE_2D, m_depth);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, 64, 64, 0,
                GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Create the cube map
        glGenTextures(1, &m_shadowMap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadowMap);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
                GL_CLAMP_TO_EDGE);

        for (uint i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, 64, 64,
                    0, GL_RED, GL_FLOAT, NULL);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                GL_TEXTURE_2D, m_depth, 0);

        // Disable writes to the color buffer
        glDrawBuffer(GL_NONE);

        // Disable reads from the color buffer
        glReadBuffer(GL_NONE);

        GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if (Status != GL_FRAMEBUFFER_COMPLETE) {
            printf("FB error, status: 0x%x\n", Status);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

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
        initP();
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
            // Shadow mapping
            world->setCam(&cam);

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, side.face, m_shadowMap, 0);
            glDrawBuffer(GL_COLOR_ATTACHMENT0);

            //shadow_fbo.setRenderFace(GL_COLOR_ATTACHMENT0, side.face);
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
        }
        world->setCam(cam);
    }
}

