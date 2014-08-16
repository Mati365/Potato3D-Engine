#ifndef EFFECTS_HPP_
#define EFFECTS_HPP_
#include <array>
#include <map>

#include "Texture.hpp"

namespace GL3Engine {
    class Material;
    class Shader {
        private:
            GLint program = 0;
            map<GLuint, GLuint> ubo; // blockindex handle do bufora
            
        public:
            Shader(c_str, c_str, c_str);

            void begin() const {
                glUseProgram(program);
            }
            void end() const {
                glUseProgram(0);
            }
            
            /** Uniformy */
            Shader& setUniform(GLint, c_str, GLint, GLuint);
            Shader& setUniform(c_str, const vector<Material*>&);

            GLuint setUBO(c_str, void*, GLuint, GLuint);
            GLuint bindToSlot(c_str, GLuint);
            void changeUBOData(GLuint, void*, size_t);

            Shader& setUniform(c_str, GLfloat);
            Shader& setUniform(c_str, GLint);

            template<GLuint len> Shader& setUniform(
                    c_str variable, const array<GLfloat, len>& array) {
                GLint loc = getUniformLoc(variable);
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
                return *this;
            }
            Shader& setUniform(c_str, const GLfloat*, GLuint, GLenum);

            Shader& setUniform(c_str, const Matrix<GLfloat>&);
            Shader& setUniform(c_str, const Vec4&);
            Shader& setUniform(c_str variable, const Color& p) {
                setUniform<4>(variable, p.toArray());
                return *this;
            }
            
            GLint getProgram() const {
                return program;
            }
            inline GLuint getUniformLoc(c_str variable) const {
                return glGetUniformLocation(program, variable.c_str());
            }
            
            ~Shader();

        private:
            void linkShader(initializer_list<GLint>);

        public:
            static GLint compileShader(c_str, GLint);
    };
}

#endif
