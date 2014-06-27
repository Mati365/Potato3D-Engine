#include "Effects.hpp"
#include "../IO.hpp"

namespace GL3Engine {
    using namespace IO;

    Shader::Shader(const string& frag, const string& vertex,
            const string& geo) {
        linkShader(
                { Shader::compileShader(frag, GL_FRAGMENT_SHADER),
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
        glLinkProgram(program);
    }
    GLint Shader::compileShader(const string& source, GLint type) {
        if (source.empty())
            return 0;

        GLint shader = glCreateShader(type);
        GLint result, length = source.length();
        char* buffer = allocString(source);

        glShaderSource(shader, 1, (const GLchar**) &buffer, &length);
        glCompileShader(shader);

        delete[] buffer;

        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            glDeleteShader(shader);
            return 0;
        }
        return shader;
    }

    /** UNIFORMY */
#define UNIFORM_LOC(variable) glGetUniformLocation(program, variable)
    void Shader::setUniform(const GLchar* variable, float value) {
        glProgramUniform1f(program,
                UNIFORM_LOC(variable),
                value);
    }
    void Shader::setUniform(const GLchar* variable, int value) {
        glProgramUniform1i(program,
                UNIFORM_LOC(variable),
                value);
    }
    void Shader::setUniform(const GLchar* variable,
            const array<GLfloat, 4>& array,
            GLuint len) {
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

    void Shader::setUniform(const GLchar* variable,
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
    void Shader::setUniform(const GLchar* variable, const FPoint3D& p) {
        glProgramUniform4f(program, UNIFORM_LOC(variable), p.X, p.Y,
                p.Z, 1.f);
    }
    void Shader::setUniform(const GLchar* variable, const Color& p) {
        glProgramUniform4f(program, UNIFORM_LOC(variable), p.r, p.g,
                p.b, p.a);
    }
}

