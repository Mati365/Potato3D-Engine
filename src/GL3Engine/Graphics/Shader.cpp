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
            return 0;
        }
        return shader;
    }
    
    /** UNIFORMY */
    Shader& Shader::setUniform(c_str variable, const Vec4& p) {
        glProgramUniform4f(
                program,
                getUniformLoc(variable),
                p.X(),
                p.Y(),
                p.Z(),
                p.W());
        return *this;
    }

    Shader& Shader::setUniform(c_str variable, GLfloat value) {
        glProgramUniform1f(program, getUniformLoc(variable), value);
        return *this;
    }
    Shader& Shader::setUniform(c_str variable, GLint value) {
        glProgramUniform1i(program, getUniformLoc(variable), value);
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
    
    Shader& Shader::setUniform(
            GLint texture_type, c_str tex, GLint flag, GLuint handle) {
        glActiveTexture(GL_TEXTURE0 + flag);
        glBindTexture(texture_type, handle);
        glProgramUniform1i(program, getUniformLoc(tex), flag);
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
    
    GLuint Shader::bindToSlot(c_str variable, GLuint binding_point) {
        GLuint block_index = glGetUniformBlockIndex(program, variable.c_str());
        glUniformBlockBinding(program, block_index, binding_point);
        // if (variable == "LightBlock") {
        //  GLint block_size;
        //    glGetActiveUniformBlockiv(program, block_index,
        //    GL_UNIFORM_BLOCK_DATA_SIZE, &block_size);
        //     const GLchar *uniformNames[1] =
        //            {
        //                    "light_count"
        //            };
        //    GLuint uniformIndices;
        //    glGetUniformIndices(program, 1, uniformNames, &uniformIndices);
        //    GLint uniformOffsets[1];
        //    glGetActiveUniformsiv(program, 1, &uniformIndices,
        //    GL_UNIFORM_OFFSET, uniformOffsets);
        // }
        return block_index;
    }
    GLuint Shader::setUBO(
            c_str variable, void* data, GLuint draw_type,
            GLuint binding_point) {
        GLuint block_index = bindToSlot(variable, binding_point);
        GLint block_size = 0;
        
        if (IS_IN_MAP(ubo, block_index))
            return ubo[block_index];
        
        glGetActiveUniformBlockiv(program, block_index,
        GL_UNIFORM_BLOCK_DATA_SIZE, &block_size);
        
        GLuint handle = genGLBuffer( {
                data, static_cast<size_t>(block_size),
                GL_UNIFORM_BUFFER, 0, draw_type }, false);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding_point, handle);
        ubo[block_index] = handle;
        
        return handle;
    }
    void Shader::changeUBOData(GLuint ubo_buf, void* data, size_t size) {
        glBindBuffer(GL_UNIFORM_BUFFER, ubo_buf);
        {
            glBufferSubData(GL_UNIFORM_BUFFER, 0, size, data);
        }
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
    
    Shader::~Shader() {
        glDeleteProgram(program);
        for (auto& handle : ubo)
            glDeleteBuffers(1, &handle.second);
    }
}

