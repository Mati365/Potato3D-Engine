#include <algorithm>

#include "../Resources/Resources.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Mesh.hpp"

namespace GL3Engine {
    namespace CoreLighting {
        using SceneObject::Camera;
        using CoreMatrix::Vec4;
        using CoreEffect::Shader;

        using CoreMaterial::CubeTexture;
        using CoreMaterial::TextureFlags;
        using CoreMaterial::Texture;
        using CoreType::genGLBuffer;

        using std::unique_ptr;
        using std::string;

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
            std::vector<LightData> data(objects.size() - 1);
            for (GLuint i = 0; i < objects.size(); ++i) {
                Light* light = objects[i];
                if (!IS_SET_FLAG(light->getType(), LightData::ENABLED))
                    continue;
                // Shadow mapy
                glCullFace(GL_FRONT);
                {
                    light->update();
                    string variable =
                            light->getHash() == CLASS_HASH(PointLight) ? "point" :
                                                                         "direct";
                    mesh_shader->setUniform(
                            light->getShadowTex()->getTexFlags().tex_type,
                            "shadow_maps[" + Tools::toString(i)
                                    + "]." + variable,
                            i + 1,
                            light->getShadowTex()->getHandle());
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
            tex = new Texture(CoreMatrix::Vec2i { 256, 256 },
                    TextureFlags { GL_RED, GL_FLOAT,
                            Texture::CLAMP_TO_EDGE | Texture::NEAREST,
                            GL_TEXTURE_2D });

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

            for (auto& side : std::make_pair(cube_cams,
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
                        node->setAttrib(SceneObject::Mesh::NONE);
                        node->draw();
                    }
                    node->popAttrib();
                }
                shadow_effect->end();
            }
            world->setCam(cam);
        }
    }
}

