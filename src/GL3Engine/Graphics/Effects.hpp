#ifndef EFFECTS_HPP_
#define EFFECTS_HPP_
#include <array>
#include <map>

#include "Texture.hpp"
#include "AttribContainer.hpp"

namespace GL3Engine {
    namespace CoreMaterial {
        class Material;
    }
    namespace CoreEffect {
        class Shader :
                       public CoreInterface::NonCopyable,
                       public CoreAttrib::ScopedContainer {
            private:
                GLint program = 0;
                std::map<GLuint, GLuint> ubo; // blockindex handle do bufora

            public:
                Shader(c_str, c_str, c_str);

                void begin() override {
                    glUseProgram(program);
                }
                void end() override {
                    glUseProgram(0);
                }

                /** Uniformy */
                Shader& setUniform(GLint, c_str, GLint, GLuint);
                Shader& setUniform(c_str,
                        const std::vector<CoreMaterial::Material*>&);

                Shader& regGlobalBuffer(GLint, GLuint, GLenum,
                        GLint* handle, void* data = nullptr,
                        c_str variable = "")
                                throw (GLint);
                Shader& bindBlockToSlot(c_str, GLuint);

                Shader& setUniform(c_str, GLfloat);
                template<GLuint len> Shader& setUniform(
                        c_str variable, const std::array<GLfloat, len>& array) {
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

                Shader& setUniform(c_str, const CoreMatrix::Matrix<GLfloat>&);
                Shader& setUniform(c_str, const CoreMatrix::Vec4&);
                Shader& setUniform(c_str, const CoreMatrix::Vec3&);
                Shader& setUniform(c_str variable, const CoreType::Color& p) {
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
                void linkShader(std::initializer_list<GLint>);

            public:
                static GLint compileShader(c_str, GLint);
        };

        class UniformBufferManager :
                                     public CoreInterface::NonCopyable,
                                     public CoreInterface::Singleton<
                                             UniformBufferManager> {
            private:
                std::map<GLuint, GLuint> ubo;

            public:
                static GLuint getBlockIndex(Shader*, c_str);
                static GLuint getBlockSize(Shader*, c_str);

                void delBuffer(GLuint);
                GLuint regBuffer(void*, GLuint, GLint, GLuint);
                void changeBufferData(GLuint, void*, size_t);

                ~UniformBufferManager() {
                    for (auto& handle : ubo)
                        glDeleteBuffers(1, &handle.second);
                }
        };
        class EffectManager :
                              public CoreAttrib::AttribContainer<Shader*>,
                              public CoreAttrib::ScopedContainer {
            public:
                struct EffectParam {
                        std::vector<GLfloat> data;
                        GLenum type;

                        GLfloat& operator[](GLuint);
                };

            private:
                std::map<std::string, EffectParam> effect_params;

            public:
                EffectManager()
                        :
                          AttribContainer(nullptr) {
                }
                EffectParam& setEffectParam(c_str, GLuint);

                void begin() override;
                void end() override;
        };
    }
}

#endif
