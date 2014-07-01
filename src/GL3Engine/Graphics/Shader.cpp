#include "Effects.hpp"
#include "Mesh.hpp"

#include "../IO.hpp"

namespace GL3Engine {
    using namespace IO;

    Shader::Shader(c_str& frag, c_str& vertex, c_str& geo) {
        linkShader(
                {
                        Shader::compileShader(frag, GL_FRAGMENT_SHADER),
                        Shader::compileShader(vertex, GL_VERTEX_SHADER),
                        Shader::compileShader(geo, GL_GEOMETRY_SHADER) });
    }

    void Shader::linkShader(initializer_list<GLint> shaders) {
        program = glCreateProgram();
        for (GLint shader : shaders)
            if (shader != 0)
                glAttachShader(program, shader);
        glBindAttribLocation(program, 0, "in_Position");
        glBindAttribLocation(program, 1, "in_Normal");
        glBindAttribLocation(program, 2, "in_UV");
        glBindAttribLocation(program, 3, "in_MTL_index");
        glLinkProgram(program);
    }
    GLint Shader::compileShader(c_str& source, GLint type) {
        if (source.empty())
            return 0;

        GLint shader = glCreateShader(type);
        GLint result, length = source.length();
        GLchar* buffer = allocString(source);

        glShaderSource(shader, 1, (const GLchar**) &buffer, &length);
        glCompileShader(shader);

        delete[] buffer;

        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            char buffer[512];
            glGetShaderInfoLog(shader, 512, nullptr, buffer);
            cout << buffer << std::endl;

            glDeleteShader(shader);
            return 0;
        }
        return shader;
    }

    /** UNIFORMY */
    void Shader::setUniform(c_str variable, GLfloat value) {
        glProgramUniform1f(program,
                UNIFORM_LOC(variable),
                value);
    }
    void Shader::setUniform(c_str variable, GLint value) {
        glProgramUniform1i(program,
                UNIFORM_LOC(variable),
                value);
    }
    void Shader::setUniform(c_str variable,
            const Matrix<GLfloat>& value) {
        GLint loc = UNIFORM_LOC(variable);
        if (value.rows == value.cols) {
            GLuint size = value.rows * value.cols;

#define MATRIX_UNIFORM(r) \
      glProgramUniformMatrix##r##fv(program, \
                  loc, \
                  1, \
                  GL_FALSE, \
                  value.matrix)

            switch (size) {
                case 16:
                    MATRIX_UNIFORM(4);
                    break;
                case 9:
                    MATRIX_UNIFORM(3);
                    break;
                case 4:
                    MATRIX_UNIFORM(2);
                    break;
            }
        } else if (value.rows == 1) {
#define MATRIX_ARRAY_UNIFORM(len) \
      glProgramUniform##len##fv(program, loc, 1, value.matrix)

            switch (value.cols) {
                case 4:
                    MATRIX_ARRAY_UNIFORM(4);
                    break;
                case 3:
                    MATRIX_ARRAY_UNIFORM(3);
                    break;
                case 2:
                    MATRIX_ARRAY_UNIFORM(2);
                    break;
            }
        }
    }

    void Shader::setUniformTexture(c_str tex, GLint flag, GLuint handle) {
        glProgramUniform1i(program, UNIFORM_LOC(tex), flag - GL_TEXTURE0);
        glActiveTexture(flag);
        glBindTexture(GL_TEXTURE_2D, handle);
    }
    void Shader::setUniform(c_str variable, const MATERIALS& material) {
        // TODO: Reszta tekstur
        for (GLuint i = 0; i < material.size(); ++i) {
            Material* mtl = material[i];
            char array_variable[50];
            sprintf(array_variable, "%s[%u]", variable.c_str(), i);

#define MATERIAL_PARAM(param) (array_variable + string(param))

            setUniform(MATERIAL_PARAM(".transparent"), mtl->transparent);
            setUniformTexture(
                    MATERIAL_PARAM(".dtex"),
                    GL_TEXTURE0 + i,
                    mtl->tex[Material::DIFFUSE] ? mtl->tex[Material::DIFFUSE]->getHandle() :
                                                  0);
        }
    }
}

