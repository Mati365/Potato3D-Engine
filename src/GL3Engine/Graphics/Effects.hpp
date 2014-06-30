#ifndef EFFECTS_HPP_
#define EFFECTS_HPP_
#include <array>

#include "GL3Engine.hpp"

namespace GL3Engine {
    class Shader {
#define UNIFORM_LOC(variable) glGetUniformLocation(program, variable)
        public:
            GLint program = 0;

            Shader(const string&, const string&, const string&);

            inline void begin() const {
                glUseProgram(program);
            }
            inline void end() const {
                glUseProgram(program);
            }

            /** Uniformy */
            void setUniform(const GLchar*, float);
            void setUniform(const GLchar*, int);
            template<GLuint len> void setUniform(const GLchar* variable,
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

            void setUniform(const GLchar*, const Matrix<GLfloat>&);
            inline void setUniform(const GLchar* variable, const FPoint3D& p) {
                glProgramUniform4f(program, UNIFORM_LOC(variable), p.X, p.Y,
                        p.Z, 1.f);
            }
            inline void setUniform(const GLchar* variable, const Color& p) {
                setUniform<4>(variable, p.toArray());
            }

            ~Shader() {
                glDeleteProgram(program);
            }

        private:
            void linkShader(initializer_list<GLint>);

        public:
            static GLint compileShader(const string&, GLint);
    };
}

#endif
