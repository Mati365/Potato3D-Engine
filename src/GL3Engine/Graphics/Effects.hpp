#ifndef EFFECTS_HPP_
#define EFFECTS_HPP_
#include <array>
#include <map>

#include "GL3Engine.hpp"

namespace GL3Engine {
    class ShaderManager;
    class Material;
    class Shader {
#define UNIFORM_LOC(variable) glGetUniformLocation(program, variable.c_str())
            friend class ShaderManager;

        private:
            GLint program = 0;

            Shader(c_str, c_str, c_str);

        public:
            inline void begin() const {
                glUseProgram(program);
            }
            inline void end() const {
                glUseProgram(program);
            }

            /** Uniformy */
            void setUniform(GLint, c_str, GLint, GLuint);
            void setUniform(c_str, const vector<Material*>&);

            void setUniform(c_str, GLfloat);
            void setUniform(c_str, GLint);
            template<GLuint len> void setUniform(c_str variable,
                    const array<GLfloat, len>& array) {
                GLint loc = UNIFORM_LOC(variable);
#define ARRAY_UNIFORM(len) \
         glProgramUniform##len##fv(program, loc, 1, &array[0])

                switch (len) {
                    case 2:
                        ARRAY_UNIFORM(2);
                        break;
                    case 3:
                        ARRAY_UNIFORM(3);
                        break;
                    case 4:
                        ARRAY_UNIFORM(4);
                        break;
                }
            }

            void setUniform(c_str, const Matrix<GLfloat>&);
            inline void setUniform(c_str variable, const FPoint3D& p) {
                glProgramUniform4f(program, UNIFORM_LOC(variable), p.X, p.Y,
                        p.Z, 1.f);
            }
            inline void setUniform(c_str variable, const Color& p) {
                setUniform<4>(variable, p.toArray());
            }

            ~Shader() {
                glDeleteProgram(program);
            }

        private:
            void linkShader(initializer_list<GLint>);

        public:
            static GLint compileShader(c_str&, GLint);
    };

    /** Prymitywny manager shaderów */
    class ShaderManager : public Singleton<ShaderManager> {
        public:
            struct ShaderInfo {
                    GLuint handle;
                    string content[3];
            };
            enum DefaultShaders {
                DEFAULT_MESH_SHADER,
                DEFAULT_TEXT_SHADER,
               // DEFAULT_BILLBOARD_SHADER
            };

        private:
            map<GLuint, Shader*> shaders;
            static ShaderInfo DEFAULT_SHADERS[];

        public:
            Shader* operator[](GLuint index) {
                return shaders[index];
            }
            void putShader(ShaderInfo&);

            void init();
            void destroy();
    };
#define GET_SHADER(handle) ShaderManager::getInstance()[handle]
}

#endif
