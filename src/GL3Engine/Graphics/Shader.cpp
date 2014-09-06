#include "Effects.hpp"
#include "Mesh.hpp"

#include "../IO.hpp"

namespace GL3Engine {
    using namespace IO;
    
    Shader::Shader(c_str frag, c_str vertex, c_str geo) {
        linkShader( {
                Shader::compileShader(frag, GL_FRAGMENT_SHADER),
                Shader::compileShader(vertex, GL_VERTEX_SHADER),
                Shader::compileShader(geo, GL_GEOMETRY_SHADER) });
    }
    
    void Shader::linkShader(initializer_list<GLint> shaders) {
        program = glCreateProgram();
        for (GLint shader : shaders)
            if (shader != 0)
                glAttachShader(program, shader);
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
            assert(0);
        }
        return shader;
    }
    
    /** UNIFORMY */
    Shader& Shader::setUniform(c_str variable, const Vec4& p) {
        setUniform(variable, p.matrix, 4, GL_FLOAT_VEC4);
        return *this;
    }
    Shader& Shader::setUniform(c_str variable, const Vec3& p) {
        setUniform(variable, p.matrix, 3, GL_FLOAT_VEC3);
        return *this;
    }

    Shader& Shader::setUniform(c_str variable, GLfloat value) {
        setUniform(variable, &value, 1, GL_FLOAT);
        return *this;
    }
    Shader& Shader::setUniform(c_str variable, const Matrix<GLfloat>& value) {
        GLint loc = getUniformLoc(variable);
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
        return *this;
    }
    
    Shader& Shader::regGlobalBuffer(GLint size, GLuint binding_point,
            GLenum draw_type, GLint* handle, void* data, c_str variable)
                    throw (GLint) {
        if (size == -1)
            size = UniformBufferManager::getBlockSize(this, variable);

        GLuint _handle = -1;
        if (size > 0)
            _handle = UniformBufferManager::getInstance().regBuffer(
                    data,
                    draw_type,
                    size,
                    binding_point);
        if (_handle == GL_INVALID_INDEX)
            throw -1;
        if (handle)
            *handle = _handle;
        return *this;
    }
    Shader& Shader::bindBlockToSlot(c_str variable, GLuint binding_point) {
        GLuint block_index = UniformBufferManager::getBlockIndex(this,
                variable);
        if (block_index == GL_INVALID_INDEX)
            return *this;
        glUniformBlockBinding(program, block_index, binding_point);
        return *this;
    }

    Shader& Shader::setUniform(
            GLint texture_type, c_str tex, GLint flag, GLuint handle) {
        GLuint loc = getUniformLoc(tex);
        if (loc == GL_INVALID_INDEX)
            return *this;

        glActiveTexture(GL_TEXTURE0 + flag);
        glBindTexture(texture_type, handle);
        glProgramUniform1i(program, loc, flag);
        glDisable(GL_TEXTURE_CUBE_MAP);

        return *this;
    }
    Shader& Shader::setUniform(c_str variable, const MATERIALS& material) {
        static char array_variable[50], col_buffer[15];
        for (GLuint i = 0; i < material.size(); ++i) {
            Material* mtl = material[i];
            sprintf(array_variable, "%s[%u]", variable.c_str(), i);
            
#define MATERIAL_PARAM(param) (array_variable + string(param))
            
            setUniform(MATERIAL_PARAM(".transparent"), mtl->transparent);
            setUniform(MATERIAL_PARAM(".shine"), mtl->shine);
            
            for (GLuint j = 0; j < Material::BUMP + 1; ++j) {
                if (j < Material::SPECULAR + 1) {
                    sprintf(col_buffer, ".%s[%u]", "col", j);
                    setUniform(MATERIAL_PARAM(col_buffer), mtl->col[j]);
                }
                sprintf(col_buffer, ".%s[%u]", "tex_flag", j);
                setUniform(MATERIAL_PARAM(col_buffer), !mtl->tex[j].empty());
            }
        }
        setUniform(GL_TEXTURE_2D_ARRAY, "texture_pack", 0,
                material[0]->tex_array->getHandle());
        return *this;
    }
    Shader& Shader::setUniform(
            c_str variable, const GLfloat* data, GLuint count, GLenum type) {
        
#define DEF_ARRAY_PASS(func_name, variables_per_obj) \
        func_name( \
                program, \
                getUniformLoc(variable), \
                count / variables_per_obj, \
                data)
#define DEF_MATRIX_ARRAY_PASS(func_name, rows, cols) \
        func_name( \
                program, \
                getUniformLoc(variable), \
                count / (rows * cols), \
                GL_FALSE, \
                data)
        
        switch (type) {
            /** SKALARY */
            case GL_FLOAT:
                DEF_ARRAY_PASS(glProgramUniform1fv, 1);
                break;
                /** WEKTORY */
#define VEC_DEF_PASS(n_count) \
            case GL_FLOAT_VEC##n_count: { \
                DEF_ARRAY_PASS(glProgramUniform##n_count##fv, n_count); \
            } break;
                {
                    VEC_DEF_PASS(2)
                    VEC_DEF_PASS(3)
                    VEC_DEF_PASS(4)
                }
                /** MACIERZE */
#define MAT_DEF_PASS(n_count) \
            case GL_FLOAT_MAT##n_count: { \
                DEF_MATRIX_ARRAY_PASS(glProgramUniformMatrix##n_count##fv, n_count, n_count); \
            } break;
                {
                    MAT_DEF_PASS(2)
                    MAT_DEF_PASS(3)
                    MAT_DEF_PASS(4)
                }
        }
        return *this;
    }

    Shader::~Shader() {
        glDeleteProgram(program);
    }
}

