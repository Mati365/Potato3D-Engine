#ifndef LIGHT_HPP_
#define LIGHT_HPP_
#include "Scene.hpp"
#include "RenderTarget.hpp"

namespace GL3Engine {
    namespace CoreLighting {
        // GLSL:
        // #define MAX_LIGHTS  10
        // struct Light {
        // vec3    pos;                            //  16B
        // vec4    specular_col;                   //  16B
        // vec4    diffuse_col;                    //  16B
        //
        // float   ambient_intensity;              //  4B
        // float   diffuse_intensity;              //  4B
        // float   specular_intensity;             //  4B
        //  4B extra
        // };
        struct LightData {
                CoreType::POS pos = { 0.f, 0.f, 0.f };
                GLfloat stride = 0.f; // randomowa liczba

                CoreType::COL spec_col = { 1.f, 1.f, 1.f, 1.f };
                CoreType::COL diff_col = { 1.f, 1.f, 1.f, 1.f };

                GLfloat ambient_intensity = 1.f;
                GLfloat diffuse_intensity = 1.f;
                GLfloat specular_intensity = 1.f;

                // extra 4B
                enum Type {
                    ENABLED = 1 << 0,
                    DISABLED = 1 << 1,
                    POINT = 1 << 2,
                    DIRECT = 1 << 3
                };
                GLint type = Type::ENABLED | Type::POINT;
        };
        class Light :
                      public CoreRenderer::Node {
            DECLARE_NODE_TYPE(Light)

            protected:
                LightData data;
                SceneObject::RenderQuad shadow_fbo;

                Light() {
                }

            public:
                virtual void update() override {
                }
                virtual const CoreMaterial::Texture* getShadowTex() const = 0;

#define ARRAY_LIGHT_SETTER(ret_type, array_size, target_variable, name) \
            virtual ret_type& set##name(const std::array<GLfloat, array_size>& array) { \
                memcpy(data.target_variable, &array[0], sizeof(GLfloat) * array_size); \
                return *this; \
            }
                ARRAY_LIGHT_SETTER(Light, 3, pos, Pos)

#define ARRAY_LIGHT_COL_SETTER(target_variable, target_intensity, name) \
            Light& set##name(const std::array<GLfloat, 4>& array, GLfloat intensity) { \
                memcpy(data.target_variable, &array[0], sizeof(GLfloat) * 4); \
                data.target_intensity = intensity; \
                return *this; \
            }
                ARRAY_LIGHT_COL_SETTER(diff_col, diffuse_intensity, Diffuse)
                ARRAY_LIGHT_COL_SETTER(spec_col, specular_intensity, Specular)

                Light& setAmbient(GLfloat intensity) {
                    this->data.ambient_intensity = intensity;
                    return *this;
                }
                GLint getType() const {
                    return data.type;
                }
                LightData& getData() {
                    return data;
                }

            protected:
                Light& setType(GLint type) {
                    data.type = type;
                    return *this;
                }
        };

        class PointLight :
                           public Light {
            DECLARE_NODE_TYPE(PointLight)

            private:
                CoreMaterial::CubeTexture* cube = nullptr;

            public:
                PointLight();
                void update() override;

                const CoreMaterial::Texture* getShadowTex() const override {
                    return cube;
                }
        };
        class DirectLight :
                            public Light {
            DECLARE_NODE_TYPE(DirectLight)

            private:
                CoreMaterial::Texture* tex = nullptr;

            public:
                DirectLight();
                void update() override;

                const CoreMaterial::Texture* getShadowTex() const override {
                    return tex;
                }
                ARRAY_LIGHT_SETTER(DirectLight, 3, pos, Dir)

            private:
                Light& setPos(const std::array<GLfloat, 3>&) {
                    return *this;
                }
        };

        // layout(std140) uniform LightBlock {
        // Light       lights[MAX_LIGHTS];
        // int         lights_count;
        // };
        class LightBatch :
                           public CoreRenderer::Batch<Light> {
            DECLARE_NODE_TYPE(LightBatch)

            public:
                static constexpr size_t MAX_LIGHTS = 10;
                static constexpr GLuint BINDING_POINT = 0;

            private:
                GLuint buffer;

            public:
                LightBatch();
                void update() override;
        };
    }
}

#endif
