#include <algorithm>

#include "../Resources/Resources.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Mesh.hpp"

namespace GL3Engine {
    namespace CoreLighting {
        TYPE_IMPORT(SceneObject, Camera);
        TYPE_IMPORT(CoreEffect, Shader);
        TYPE_IMPORT(CoreMatrix, Vec4);

        TYPE_IMPORT(CoreMaterial, CubeTexture);
        TYPE_IMPORT(CoreMaterial, TextureFlags);
        TYPE_IMPORT(CoreMaterial, Texture);
        TYPE_IMPORT(GPU, Allocator);
        TYPE_IMPORT(CoreEffect, UniformBufferManager);

        TYPE_IMPORT(std, unique_ptr);
        TYPE_IMPORT(std, string);

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

        PointLightCam cube_cams[] =
                {
                DEF_FACE(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 1.f, 0.f, 0.f, 0.f, -1.f, 0.f),
                DEF_FACE(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, -1.f, 0.f, 0.f,0.f, -1.f, 0.f),

                DEF_FACE(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0.f, 1.f, 0.f, 0.f, 0.f, -1.f),
                DEF_FACE(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0.f, -1.f, 0.f,0.f, 0.f, 1.f),

                DEF_FACE(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0.f, 0.f, 1.f, 0.f, -1.f, 0.f),
                DEF_FACE(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0.f, 0.f, -1.f, 0.f, -1.f, 0.f),
                };
        LightBatch::LightBatch() {
            buffer = UniformBufferManager::getInstance().regBuffer(
                    nullptr,
                    GL_STREAM_DRAW,
                    sizeof(LightData) * MAX_LIGHTS + sizeof(GLfloat),
                    BINDING_POINT);
        }
        void LightBatch::update() {
            if (objects.empty())
                return;
            // Wysyłanie bloku informacji o pozycji dir itp
            std::vector<LightData> data(objects.size() - 1);
            Shader *mesh_shader = REQUIRE_RES(Shader, DEFAULT_MESH_SHADER),
                    *shadow_shader = REQUIRE_RES(Shader, DEFAULT_SHADOW_SHADER);

            shadow_shader->begin();
            for (GLuint i = 0; i < objects.size(); ++i) {
                Light* light = objects[i];
                if (!IS_SET_FLAG(light->getType(), LightData::ENABLED))
                    continue;
                glCullFace(GL_FRONT);
                {
                    light->update();
                    string variable =
                            light->getHash() == CLASS_HASH(PointLight) ? "point" :
                                                                         "direct";
                    mesh_shader->setUniform(
                            light->getShadowTex()->getTexFlags().tex_type,
                            "shadow_map[" + std::to_string(i) + "]." + variable,
                            i + 1,
                            light->getShadowTex()->getHandle());
                }
                glCullFace(GL_BACK);
                data.push_back(light->getData());
            }
            shadow_shader->end();
            // Wysyłanie pozycji świateł
            GLfloat size = data.size();
            UniformBufferManager::getInstance()
                    .changeBufferData(BINDING_POINT, &data[0],
                    data.size() * sizeof(LightData))
                    .changeBufferData(BINDING_POINT, &size, sizeof(GLfloat),
                    MAX_LIGHTS * sizeof(LightData));
            SET_SCENE_FLAG(scene, LIGHT_SHADER_BINDING, BINDING_POINT);
        }

        // ---- DirectLight
        DirectLight::DirectLight() {
            tex = new Texture(CoreMatrix::Vec2i { 256, 256 },
                    TextureFlags { GL_RED, GL_FLOAT,
                            Texture::CLAMP_TO_EDGE | Texture::NEAREST,
                            GL_TEXTURE_2D
                    });

            shadow_fbo.attachTex(GL_COLOR_ATTACHMENT0,
                    tex,
                    GL_TEXTURE_2D);
            shadow_fbo.setSize(CoreMatrix::Vec2i { 256, 256 });
            setType(LightData::ENABLED | LightData::DIRECT);
        }
        void DirectLight::update() {
        }

        // ---- PointLight
        PointLight::PointLight() {
            cube = new CubeTexture( { 256, 256 },
                    TextureFlags { GL_RGBA, GL_UNSIGNED_BYTE,
                            Texture::CLAMP_TO_EDGE | Texture::NEAREST,
                            GL_TEXTURE_CUBE_MAP
                    });
            shadow_fbo.setSize(cube->getSize());
            shadow_fbo.attachDepthTex();
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
            for (auto& side : std::make_pair(cube_cams,
                    cube_cams + CubeTexture::CUBE_TEX_FACES)) {
                Camera _cam = {
                        pos + side.cam->getPos(),
                        pos + side.cam->getTarget()
                };
                world->setCam(&_cam);
                shadow_fbo.setRenderFace(GL_COLOR_ATTACHMENT0, cube, side.face);
                for (auto& node : *scene) {
                    if (node->getHash() != CLASS_HASH(Mesh))
                        continue;
                    node->pushAttrib();
                    node->setAttrib(SceneObject::Mesh::Flags::NORMAL);
                    node->draw();
                    node->popAttrib();
                }
            }
            world->setCam(cam);
        }
    }
}

