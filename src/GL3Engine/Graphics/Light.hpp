#ifndef LIGHT_HPP_
#define LIGHT_HPP_
#include "Effects.hpp"

namespace GL3Engine {
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
    struct Light {
            POS pos;
            GLfloat stride;

            COL spec_col;
            COL diff_col;

            GLfloat ambient_intensity;
            GLfloat diffuse_intensity;
            GLfloat specular_intensity;

            // Dodatkowe 4 bajtów
            enum State {
                ON, OFF
            };
            GLfloat state;
    };

    // layout(std140) uniform LightBlock {
    // Light       lights[MAX_LIGHTS];
    // int         lights_count;
    // };
    class LightManager : public Singleton<LightManager> {
        public:
            static constexpr size_t MAX_LIGHTS = 10;
            static constexpr GLuint BINDING_POINT = 0;

        private:
            vector<Light> lights;
            GLuint buffer;

        public:
            LightManager() {
                create();
            }

            void update();
            void addLight(const Light& light) {
                lights.push_back(light);
            }

        private:
            void create();
    };
}

#endif
