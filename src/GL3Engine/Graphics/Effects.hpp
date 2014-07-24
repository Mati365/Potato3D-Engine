#ifndef EFFECTS_HPP_
#define EFFECTS_HPP_
#include <array>
#include <map>
#include <GL/glew.h>
#include <GL/glu.h>

#include "Texture.hpp"

namespace GL3Engine {
    class Material;
    class Shader {
#define UNIFORM_LOC(variable) glGetUniformLocation(program, variable.c_str())

        private:
            GLint program = 0;
            map<GLuint, GLuint> ubo; // blockindex handle do bufora

        public:
            Shader(c_str, c_str, c_str);

            inline void begin() const {
                glUseProgram(program);
            }
            inline void end() const {
                glUseProgram(0);
            }

            /** Uniformy */
            void setUniform(GLint, c_str, GLint, GLuint);
            void setUniform(c_str, const vector<Material*>&);

            GLuint setUBO(c_str, void*, GLuint, GLuint);
            GLuint bindToSlot(c_str, GLuint);
            void changeUBOData(GLuint, void*, size_t);

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

            inline GLint getProgram() const {
                return program;
            }

            ~Shader();

        private:
            void linkShader(initializer_list<GLint>);

        public:
            static GLint compileShader(c_str, GLint);
    };
}

#endif
