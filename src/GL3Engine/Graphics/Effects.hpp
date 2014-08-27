#ifndef EFFECTS_HPP_
#define EFFECTS_HPP_
#include <array>
#include <map>

#include "Texture.hpp"
#include "AttribContainer.hpp"

namespace GL3Engine {
    class Material;
    class Shader :
                   public NonCopyable,
                   public ScopedContainer {
        private:
            GLint program = 0;
            map<GLuint, GLuint> ubo; // blockindex handle do bufora
            
        public:
            Shader(c_str, c_str, c_str);

            void begin() const override {
                glUseProgram(program);
            }
            void end() const override {
                glUseProgram(0);
            }
            
            /** Uniformy */
            Shader& setUniform(GLint, c_str, GLint, GLuint);
            Shader& setUniform(c_str, const vector<Material*>&);

            Shader& regGlobalBuffer(GLint, GLuint, GLenum,
                    GLint* handle, void* data = nullptr, c_str variable = "")
                            throw (GLint);
            Shader& bindBlockToSlot(c_str, GLuint);

            Shader& setUniform(c_str, GLfloat);
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
            Shader& setUniform(c_str, const Vec3&);
            Shader& setUniform(c_str variable, const Color& p) {
                setUniform<4>(variable, p.toArray());
                return *this;
            }
            
            GLint getProgram() const {
                return program;
            }
            inline GLint getUniformLoc(c_str variable) const {
                return glGetUniformLocation(program, variable.c_str());
            }
            
            ~Shader();

        private:
            void linkShader(initializer_list<GLint>);

        public:
            static GLint compileShader(c_str, GLint);
    };
    class UniformBufferManager :
                                 public NonCopyable,
                                 public Singleton<UniformBufferManager> {
        private:
            map<GLuint, GLuint> ubo;

        public:
            static GLint getBlockIndex(Shader*, c_str);
            static GLint getBlockSize(Shader*, c_str);

            void delBuffer(GLint);
            GLuint regBuffer(void*, GLuint, GLint, GLuint);
            void changeBufferData(GLint, void*, size_t);

            ~UniformBufferManager() {
                for (auto& handle : ubo)
                    glDeleteBuffers(1, &handle.second);
            }
    };
}

#endif
