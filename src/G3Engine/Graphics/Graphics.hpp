/*
 * Graphics.hpp
 *
 *  Created on: 27 cze 2014
 *      Author: mateusz
 */

#ifndef GRAPHICS_HPP_
#define GRAPHICS_HPP_
#include <stdio.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SOIL/SOIL.h>

#include <initializer_list>
#include <list>
#include <vector>
#include <array>
#include <map>

#include <math.h>

#include <sstream>
#include <stdio.h>

#include <boost/scoped_ptr.hpp>

#include "../IO.hpp"
#include "Matrix.hpp"
#include "Types.hpp"
#include "Math.hpp"

namespace Graphics {
    using namespace boost;

    class MatrixStack {
        public:
            Mat4 projection,
                    view,
                    model,
                    vp_matrix; // cache z mnożenia view * projection

            struct M_STACK_ARRAY {
                    GLfloat array[16];
            };
            list<M_STACK_ARRAY> stack;

            MatrixStack();
            void setCameraCoords(const Camera&);

            void pushTransform();
            void popTransform();
    };
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

    /** Tekstury */
    template<typename T> GLint genGLBuffer(const T* data, GLuint len,
            GLint type) {
        GLuint buffer = 0;
        if (data == nullptr)
            return buffer;

        glGenBuffers(1, &buffer);
        glBindBuffer(type, buffer);
        glBufferData(type, len, data, GL_STATIC_DRAW);

        return buffer;
    }

    class Texture {
        private:
            GLuint handle;
            IPoint2D size;

        public:
            Texture(const string&);

            GLuint getHandle() const {
                return handle;
            }
            IPoint2D& getSize() {
                return size;
            }

            ~Texture() {
                glDeleteTextures(1, &handle);
            }

        private:
            void configure();
    };
    class Drawable {
        public:
            virtual void draw(MatrixStack&, GLint)=0;
            virtual ~Drawable() {
            }
    };
    class Shape : public Drawable {
        public:
            GLuint vao = 0,
                    vbo = 0,
                    indices = 0,
                    vertices_count = 0,
                    indices_count = 0;
            Color col;

            Shape(const Vertex* buffer,
                    GLint vertices,
                    const GLushort* i_buffer,
                    GLint indices,
                    const Color& _col)
                    :
                      col(_col) {
                create(buffer, vertices, i_buffer, indices);
            }
            Shape(const Vertex* buffer,
                    GLint vertices,
                    const GLushort* i_buffer,
                    GLint indices) {
                create(buffer, vertices, i_buffer, indices);
            }

            void setColor(const Color& col) {
                this->col = col;
            }
            void draw(MatrixStack&, GLint);

            ~Shape() {
                glDeleteBuffers(1, &vbo);
            }

        private:
            void create(const Vertex*, GLint, const GLushort*, GLint);
    };
}

#endif /* GRAPHICS_HPP_ */
