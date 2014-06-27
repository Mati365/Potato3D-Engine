#ifndef EFFECTS_HPP_
#define EFFECTS_HPP_
#include <array>

#include "GL3Engine.hpp"

namespace GL3Engine {
    class Shader {
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
            void setUniform(const GLchar*, const array<GLfloat, 4>&y, GLuint);

            void setUniform(const GLchar*, const Matrix<GLfloat>&);
            inline void setUniform(const GLchar*, const FPoint3D&);
            inline void setUniform(const GLchar*, const Color&);

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
