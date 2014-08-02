#ifndef LIGHT_HPP_
#define LIGHT_HPP_
#include "Scene.hpp"

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
    struct LightData {
            POS pos = { 0.f, 0.f, 0.f };
            GLfloat stride = 0.f; // randomowa liczba

            COL spec_col = { 1.f, 1.f, 1.f, 1.f };
            COL diff_col = { 1.f, 1.f, 1.f, 1.f };

            GLfloat ambient_intensity = 1.f;
            GLfloat diffuse_intensity = 1.f;
            GLfloat specular_intensity = 1.f;

            // extra 4B
            enum State {
                ON, OFF
            };
            GLfloat state = State::ON;
    };
    class Light : public Node {
        private:
            LightData data;

        public:
            void draw();

#define ARRAY_LIGHT_SETTER(ret_type, array_size, target_variable, name) \
            ret_type& set##name(const array<GLfloat, array_size>& array) { \
                memcpy(data.target_variable, &array[0], sizeof(GLfloat) * array_size); \
                return *this; \
            }
            ARRAY_LIGHT_SETTER(Light, 3, pos, Pos)

#define ARRAY_LIGHT_COL_SETTER(target_variable, target_intensity, name) \
            Light& set##name(const array<GLfloat, 4>& array, GLfloat intensity) { \
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

            Light& setState(LightData::State state) {
                data.state = state;
                return *this;
            }
            GLfloat getState() const {
                return data.state;
            }
            LightData& getData() {
                return data;
            }
    };

    // layout(std140) uniform LightBlock {
    // Light       lights[MAX_LIGHTS];
    // int         lights_count;
    // };
    template<typename T>
    class Batch : public Node {
        protected:
            vector<T*> objects;

        public:
            Batch& regObject(T& object) {
                if (is_base_of<Node, T>::value)
                    dynamic_cast<Node*>(&object)->setParent(this);
                objects.push_back(&object);
                return *this;
            }
    };
    class LightBatch : public Batch<Light> {
        public:
            static constexpr size_t MAX_LIGHTS = 10;
            static constexpr GLuint BINDING_POINT = 0;

        private:
            GLuint buffer;

        public:
            LightBatch() {
                createBuffer();
            }
            void draw();

        private:
            void createBuffer();
    };
}

#endif
