#include <iostream>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <initializer_list>
#include <deque>
#include <list>
#include <vector>

#include <math.h>

#include <sstream>
#include <fstream>
#include <stdio.h>

#include <boost/scoped_ptr.hpp>

using namespace std;

namespace Tools {
    typedef unsigned short int UINT;

#define IS_SET_FLAG(num, flag) (num & flag)
#define ARRAY_LENGTH(type, array) (sizeof(array) / sizeof(type))
#define TO_RAD(angle) (angle * 180.0 / 3.145)

    template<typename T> class Point3D {
        public:
            T X = 0,
                    Y = 0,
                    Z = 0;

            Point3D() {
            }
            Point3D(const T& _X, const T& _Y, const T& _Z)
                    :
                      X(_X),
                      Y(_Y),
                      Z(_Z) {
            }

            Point3D<T>& operator+=(const Point3D<T>& v) {
                X += v.X;
                Y += v.Y;
                Z += v.Z;
                return *this;
            }
            Point3D<T>& operator-=(const Point3D<T>& v) {
                X -= v.X;
                Y -= v.Y;
                Z -= v.Z;
                return *this;
            }
            Point3D<T>& operator*=(const Point3D<T>& v) {
                this->X *= v.X;
                this->Y *= v.Y;
                this->Z *= v.Z;
                return *this;
            }

            inline T getVecLength() const {
                return sqrt(pow(this->X, 2) + pow(this->Y, 2) + pow(this->Z, 2));
            }
            Point3D<T>& normalize() {
                T length = getVecLength();
                this->X /= length;
                this->Y /= length;
                this->Z /= length;
                return *this;
            }
    };

#define VEC_OVERLOAD(oper) \
	template<typename T> Point3D<T> operator oper (const Point3D<T>& l, \
			const Point3D<T>& p) { \
		Point3D<T> _l = l; \
		_l oper##= p; \
		return _l; \
	}
    VEC_OVERLOAD(+)
    VEC_OVERLOAD(-)
    VEC_OVERLOAD(*)

    template<typename T> inline Point3D<T> normalize(const Point3D<T>& _p) {
        return Point3D<T>(_p).normalize();
    }
    template<typename T> Point3D<T> cross(const Point3D<T>& a,
            const Point3D<T>& b) {
        return {
            a.Y * b.Z - a.Z * b.Y,
            a.Z * b.X - a.X * b.Z,
            a.X * b.Y - a.Y * b.X
        };
    }
    template<typename T> inline T dot(const Point3D<T>& a,
            const Point3D<T>& b) {
        return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
    }
    template<typename T> class Point2D : public Point3D<T> {
        public:
            Point2D(const T& _X, const T& _Y)
                    :
                      Point3D<T>(_X, _Y, 0) {
            }
    };

    using IPoint2D = Point2D<GLint>;
    using FPoint2D = Point2D<GLfloat>;
    using FPoint3D = Point3D<GLfloat>;

#define BOX_DEFINE(dimensions) \
    template<typename T> class Box##dimensions##D { \
        public: \
            Point##dimensions##D<T> pos, \
                    size; \
            Box##dimensions##D() {} \
            Box##dimensions##D(const Point##dimensions##D<T>& _pos, const Point##dimensions##D<T>& _size) \
                    : \
                      pos(_pos), \
                      size(_size) { \
            } \
    }
    BOX_DEFINE(2);
    BOX_DEFINE(3);

    template<typename T> class Rect : public Point2D<T> {
        public:
            T W, H;

            Rect(const T& _X, const T& _Y, const T& _W, const T& _H)
                    :
                      Point2D<T>(_X, _Y),
                      W(_W),
                      H(_H) {
            }
            inline T getSize() const {
                return this->X * this->Y;
            }
    };
    template<typename T> void safe_delete(T*& ptr, bool arr) {
        if (ptr == nullptr)
            return;

        if (arr)
            delete[] ptr;
        else
            delete ptr;
        ptr = nullptr;
    }

    struct Log {
            enum Flag {
                CRITICAL,
                WARNING,
                ERROR
            };

            Flag flag;
            string message;

            static deque<Log> logs;
    };
    deque<Log> Log::logs;
    void showGLErrors() {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR)
            cout << "OpenGL error: " << err << endl;
    }
    template<typename T> class Singleton {
        public:
            static T& getInstance() {
                static T t;
                return t;
            }
    };
}
namespace IO {
    using namespace Tools;

    inline size_t getFileLength(const string& path) {
        return ifstream(path, ios::binary | ios::ate).tellg();
    }
    string getFileContents(const string& path) {
        ifstream t(path);
        stringstream buffer;
        buffer << t.rdbuf();
        return buffer.str();
    }
    char* allocString(const string& str) {
        char* buf = new char[str.length()];
        for (UINT i = 0; i < str.length(); ++i)
            buf[i] = str[i];
        return buf;
    }
}
namespace Graphics {
    using namespace IO;
    using namespace boost;

    template<typename T> class Matrix {
        public:
            T* matrix = nullptr;
            UINT cols = 0,
                    rows = 0;

            Matrix(const Matrix<T>& matrix) {
                *this = matrix;
            }
            Matrix(UINT _cols, UINT _rows)
                    :
                      cols(_cols),
                      rows(_rows) {
                matrix = new T[rows * cols];
                memset(matrix, 0, rows * cols * sizeof(GLfloat));
            }
            Matrix(UINT _cols, UINT _rows, const initializer_list<T>& array)
                    :
                      Matrix<T>(_cols, _rows) {
                for (UINT i = 0; i < array.size(); ++i)
                    matrix[i] = *(array.begin() + i);
            }
            void print() const {
                for (UINT i = 0; i < rows; ++i) {
                    for (UINT j = 0; j < cols; ++j)
                        cout << matrix[i * cols + j] << " ";
                    cout << endl;
                }
            }

            /** Operacje na macierzach */
            Matrix<T>& operator*=(T k) {
                for (UINT i = 0; i < rows * cols; ++i)
                    matrix[i] *= k;
                return *this;
            }
            Matrix<T>& operator*=(const Matrix<T>& matrix) {
                T* temp = new T[rows * matrix.cols];
                for (UINT i = 0; i < matrix.cols; ++i)
                    for (UINT j = 0; j < rows; ++j) {
                        T sum = 0;
                        for (UINT k = 0; k < cols; ++k)
                            sum += this->matrix[j * cols + k]
                                    * matrix.matrix[k * matrix.cols + i];
                        temp[j * matrix.cols + i] = sum;
                    }

                safe_delete<GLfloat>(this->matrix, true);

                cols = matrix.cols;
                this->matrix = temp;

                return *this;
            }
            Matrix<T>& operator+=(const Matrix<T>& matrix) {
                for (UINT i = 0; i < rows; ++i)
                    for (UINT j = 0, index = 0; j < cols;
                            ++j, index = i * cols + j)
                        this->matrix[index] += *matrix.matrix[index];
                return *this;
            }
            Matrix<T>& operator=(const Matrix<T>& matrix) {
                if (rows != matrix.rows && cols != matrix.cols) {
                    rows = matrix.rows;
                    cols = matrix.cols;

                    safe_delete<GLfloat>(this->matrix, true);
                    this->matrix = new T[rows * cols];
                }
                memcpy(this->matrix, matrix.matrix,
                        rows * cols * sizeof(GLfloat));
                return *this;
            }
            Matrix<T>& operator=(const T* dynamic_array) {
                safe_delete<GLfloat>(this->matrix, true);
                this->matrix = dynamic_array;
                return *this;
            }

            operator Point3D<T>() const {
                return {
                    matrix[0],
                    matrix[1],
                    matrix[2]
                };
            }
            operator Point2D<T>() const {
                return {
                    matrix[0],
                    matrix[1]
                };
            }

            T* get(UINT x, UINT y) {
                return matrix[y * cols + x];
            }
            T& operator[](UINT i) {
                return matrix[i];
            }
            inline GLint getLength() const {
                return rows * cols;
            }

            ~Matrix() {
                safe_delete(matrix, true);
            }
    };
    template<typename T> Matrix<T> operator*(const Matrix<T>& l,
            const Matrix<T>& r) {
        Matrix<T> temp = l;
        temp *= r;
        return temp;
    }
    template<typename T, UINT COLS, UINT ROWS> class t_Matrix : public Matrix<T> {
        public:
            t_Matrix()
                    :
                      Matrix<T>(COLS, ROWS) {
            }
            t_Matrix(const initializer_list<T>& array)
                    :
                      Matrix<T>(COLS, ROWS, array) {
            }

            t_Matrix<T, COLS, ROWS>& operator=(const Matrix<T>& matrix) {
                Matrix<T>::operator =(matrix);
                return *this;
            }
    };

    using Mat4 = t_Matrix<GLfloat, 4, 4>;
    using Mat3 = t_Matrix<GLfloat, 3, 3>;
    using Mat2 = t_Matrix<GLfloat, 2, 2>;

    using Vec4 = t_Matrix<GLfloat, 4, 1>;
    using Vec3 = t_Matrix<GLfloat, 3, 1>;
    using Vec2 = t_Matrix<GLfloat, 2, 1>;

    /** TODO:
     *  Macierze powinny być predefiniowane
     *  a nie tworzone na nowo
     */
    template<typename T> class MatMatrix {
        public:
            static inline GLfloat cotan(GLfloat rad) {
                return 1.f / tan(rad);
            }

            /** Operacje na macierzy macierzy [ x, y, z, w ] */
            static constexpr Mat4 identity() {
                return Mat4( {
                        1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1
                });
            }
            static inline void identity(Matrix<T>& matrix) {
                matrix = identity();
            }

            static Mat4 translate(const FPoint3D& v) {
                return Mat4( {
                        1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        v.X, v.Y, v.Z, 1
                });
            }
            static inline void translate(Matrix<T>& matrix,
                    const FPoint3D& v) {
                matrix *= translate(v);
            }

            static Mat4 scale(const FPoint3D& scale) {
                return Mat4( {
                        scale.X, 0, 0, 0,
                        0, scale.Y, 0, 0,
                        0, 0, scale.Z, 0,
                        0, 0, 0, 1
                });
            }
            static inline void scale(Matrix<T>& matrix,
                    const FPoint3D& _scale) {
                matrix *= scale(_scale);
            }

            static Mat4 rotate(GLfloat theta,
                    const FPoint3D& axis) {
                float c = cosf(theta), s = sinf(theta), m_c = 1.0 - c;
                return Mat4( {
                        // Wers 1
                        (float) pow(axis.X, 2) * m_c + c,
                        axis.X * axis.Y * m_c - axis.Z * s,
                        axis.X * axis.Z * m_c + axis.Y * s,
                        0,
                        // Wers 2
                        axis.X * axis.Y * m_c + axis.Z * s,
                        (float) pow(axis.Y, 2) * m_c + c,
                        axis.Y * axis.Z * m_c - axis.X * s,
                        0,
                        // Wers 3
                        axis.X * axis.Z * m_c - axis.Y * s,
                        axis.Y * axis.Z * m_c + axis.X * s,
                        (float) pow(axis.Z, 2) * m_c + c,
                        0,
                        // Wers 4
                        0, 0, 0, 1
                });
            }
            static inline void rotate(Matrix<T>& matrix, GLfloat theta,
                    const FPoint3D& axis) {
                matrix *= rotate(theta, axis);
            }

            /**
             * Obiliczenia dla MVP
             * Wzory:
             * http://www.3dcpptutorials.sk/index.php?id=2
             */
            static Mat4 perspective(GLfloat angle, GLfloat aspect, GLfloat near,
                    GLfloat far) {
                GLfloat c = cotan(angle / 2.0);
                return Mat4(
                        {
                                c / aspect, 0, 0, 0,
                                0, c, 0, 0,
                                0, 0, (far + near) / (near - far), (2 * far
                                        * near) / (near - far),
                                0, 0, -1, 0
                        });
            }
            static Mat4 lookAt(const FPoint3D& _eye,
                    const FPoint3D& _target,
                    const FPoint3D& _dir) {
                FPoint3D z_c = normalize(_eye - _target);
                FPoint3D y_c = normalize(_dir);
                FPoint3D x_c = normalize(cross(y_c, z_c));
                y_c = cross(z_c, x_c);

                return Mat4( {
                        x_c.X, x_c.Y, x_c.Z, -dot(x_c, _eye),
                        y_c.X, y_c.Y, y_c.Z, -dot(y_c, _eye),
                        z_c.X, z_c.Y, z_c.Z, -dot(z_c, _eye),
                        0, 0, 0, 1
                });
            }
    };

    using FMAT_MATH = Graphics::MatMatrix<GLfloat>;
    using IMAT_MATH = Graphics::MatMatrix<GLint>;

    /**
     * MatrixStack zarządza stosem OGla,
     * nie wykonuje operacji matematycznych
     * tym zajmuje się MatMatrix
     */
    struct Camera {
            Vec4 pos = { 0.f, 0.f, 2.5f, 1.f };
            Vec4 target = { 0.f, 0.f, 0.f, 1.f };
    };
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

            MatrixStack() {
                projection = FMAT_MATH::perspective(90.f, 1.f / 1.f, 1.f,
                        100.f);
                model = FMAT_MATH::identity();
            }
            void setCameraCoords(const Camera& camera) {
                view = FMAT_MATH::lookAt(
                        camera.pos,
                        camera.target,
                        { 0, 1, 0 });
                vp_matrix = projection * view;
            }

            void pushTransform() {
                M_STACK_ARRAY array;
                memcpy(array.array, model.matrix, 16 * sizeof(GLfloat));
                stack.push_back(array);
            }
            void popTransform() {
                if (stack.empty())
                    return;
                memcpy(model.matrix, stack.back().array, 16 * sizeof(GLfloat));
                stack.pop_back();
            }
    };

    class Shader {
        public:
            GLint program = 0;

            Shader(const string& frag, const string& vertex,
                    const string& geo) {
                linkShader(
                        { Shader::compileShader(frag, GL_FRAGMENT_SHADER),
                                Shader::compileShader(vertex, GL_VERTEX_SHADER),
                                Shader::compileShader(geo, GL_GEOMETRY_SHADER) });
            }

            inline void begin() const {
                glUseProgram(program);
            }
            inline void end() const {
                glUseProgram(program);
            }

            /** Uniformy */
#define UNIFORM_LOC(variable) glGetUniformLocation(program, variable)
            void setUniform(const char* variable, float value) {
                glProgramUniform1f(program,
                        UNIFORM_LOC(variable),
                        value);
            }
            void setUniform(const char* variable, int value) {
                glProgramUniform1i(program,
                        UNIFORM_LOC(variable),
                        value);
            }
            void setUniform(const char* variable,
                    const Matrix<GLfloat>& value) {
                GLint loc = UNIFORM_LOC(variable);
                if (value.rows == value.cols) {
                    UINT size = value.rows * value.cols;
                    switch (size) {
                        /** Matrix 4x4 */
                        case 16:
                            glProgramUniformMatrix4fv(program,
                                    loc,
                                    1,
                                    GL_FALSE,
                                    value.matrix);
                            break;
                            /** Matrix3x3 */
                        case 9:
                            glProgramUniformMatrix3fv(program,
                                    loc,
                                    1,
                                    GL_FALSE,
                                    value.matrix);
                            break;

                            /** Matrix2x2 */
                        case 4:
                            glProgramUniformMatrix2fv(program,
                                    loc,
                                    1,
                                    GL_FALSE,
                                    value.matrix);
                            break;
                    }
                } else if (value.rows == 1) {
                    switch (value.cols) {
                        /** Vec4 */
                        case 4:
                            glProgramUniform4fv(program, loc, 1, value.matrix);
                            break;
                            /** Vec3 */
                        case 3:
                            glProgramUniform3fv(program, loc, 1, value.matrix);
                            break;
                            /** Vec2 */
                        case 2:
                            glProgramUniform2fv(program, loc, 1, value.matrix);
                            break;
                    }
                }
            }
            void setUniform(const char* variable,
                    const FPoint2D& vec) {
                glProgramUniform2f(
                        program,
                        UNIFORM_LOC(variable),
                        vec.X,
                        vec.Y);
            }
            void setUniform(const char* variable,
                    const FPoint3D& vec) {
                glProgramUniform3f(
                        program,
                        UNIFORM_LOC(variable),
                        vec.X,
                        vec.Y,
                        vec.Z);
            }

            ~Shader() {
                glDeleteProgram(program);
            }

        private:
            void linkShader(initializer_list<GLint> shaders) {
                program = glCreateProgram();
                for (GLint shader : shaders)
                    if (shader != 0)
                        glAttachShader(program, shader);
                glBindAttribLocation(program, 0, "in_Position");
                glBindAttribLocation(program, 1, "in_Color");
                glLinkProgram(program);
            }

        public:
            static GLint compileShader(const string&, GLint);
    };
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

    class Drawable {
        public:
            virtual void draw(MatrixStack&, GLint)=0;
            virtual ~Drawable() {
            }
    };
    struct Vertex {
            GLfloat pos[4];
            GLfloat col[4];
            GLfloat normal[3];
            GLfloat uv[2];
    };

    template<typename T> GLint genGLBuffer(const T* data, GLuint len,
            GLint type) {
        GLuint buffer = 0;
        if (data == nullptr)
            return buffer;

        glGenBuffers(1, &buffer);
        glBindBuffer(type, buffer);
        glBufferData(type, len * sizeof(T), data, GL_STATIC_DRAW);

        return buffer;
    }
    class Shape : public Drawable {
        public:
            GLuint vao = 0,
                    vbo = 0,
                    indices = 0,
                    vertices_count = 0,
                    indices_count = 0;

            Shape(const Vertex* buffer, GLint vertices,
                    const GLushort* i_buffer,
                    GLint indices) {
                create(buffer, vertices, i_buffer, indices);
            }
            void draw(MatrixStack& matrix, GLint mode) {
                static Shader shader(
                        getFileContents("shaders/fragment_shader.txt"),
                        getFileContents("shaders/vertex_shader.txt"),
                        "");

                shader.begin();
                shader.setUniform("mvp", matrix.vp_matrix * matrix.model);
                {
                    glBindVertexArray(vao);
                    if (!indices)
                        glDrawArrays(mode, 0, vertices_count);
                    else
                        glDrawElements(mode, indices_count, GL_UNSIGNED_SHORT,
                                nullptr);
                    glBindVertexArray(0);
                }
                shader.end();
            }
            ~Shape() {
                glDeleteBuffers(1, &vbo);
            }

        private:
            void create(const Vertex* buffer, GLint vertices,
                    const GLushort* i_buffer,
                    GLint indices) {
                glGenVertexArrays(1, &vao);
                glBindVertexArray(vao);

                // Generowani bufora indeksow
                if (i_buffer != nullptr) {
                    this->indices = genGLBuffer<GLushort>(
                            i_buffer,
                            indices,
                            GL_ELEMENT_ARRAY_BUFFER);
                    this->indices_count = indices;
                }

                // Generowanie bufora wierzcholkow
                vbo = genGLBuffer<Vertex>(
                        buffer,
                        vertices * sizeof(Graphics::Vertex) / sizeof(GLfloat),
                        GL_ARRAY_BUFFER);
                this->vertices_count = vertices;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#define VERTEX_ATTR_PTR(loc, count, strip) \
	glVertexAttribPointer(loc, count, GL_FLOAT, GL_FALSE, sizeof(Vertex), BUFFER_OFFSET(strip * sizeof(GLfloat))); \
	glEnableVertexAttribArray(loc)

                VERTEX_ATTR_PTR(0, 4, 0); // Vertex
                VERTEX_ATTR_PTR(1, 4, 4); // Colors
                VERTEX_ATTR_PTR(2, 3, 8); // Normals
                VERTEX_ATTR_PTR(3, 2, 11); // UVs

                glBindVertexArray(0);
            }
    };
}
namespace Engine {
    using namespace Graphics;
    /** Rendering prymitywów */
    class Primitives {
        public:
            static Shape* genAxis(GLfloat size) {
                if (!size % 2)
                    return nullptr;

                vector<Vertex> sheet;
                GLfloat start_pos = floor(sqrt(size));
                for (GLint i = 0; i < (GLint) size; ++i) {
                    sheet.push_back( {
                            { i * .5f - start_pos, 0.f, -start_pos, 1.f },
                            { .25f, .25f, .25f, 1.f },
                            { 1.f, 1.f, 1.f },
                            { 0.f, 0.f }
                    });
                    sheet.push_back( {
                            { i * .5f - start_pos, 0.f, start_pos, 1.f },
                            { .25f, .25f, .25f, 1.f },
                            { 1.f, 1.f, 1.f },
                            { 0.f, 0.f }
                    });
                    sheet.push_back( {
                            { start_pos, 0.f, -start_pos + i * .5f, 1.f },
                            { .25f, .25f, .25f, 1.f },
                            { 1.f, 1.f, 1.f },
                            { 0.f, 0.f }
                    });
                    sheet.push_back( {
                            { -start_pos, 0.f, -start_pos + i * .5f, 1.f },
                            { .25f, .25f, .25f, 1.f },
                            { 1.f, 1.f, 1.f },
                            { 0.f, 0.f }
                    });
                }
                return new Shape(&sheet[0], sheet.size(), nullptr, 0);
            }
    };

    /**
     * Zastosowanie kompozycji,
     * dziedziczenie odpada bo
     * w trakcie życia meshu
     * będzie można zmieniać mu
     * siatki
     */
    class Mesh : public Drawable {
        public:
            Shape* shape = nullptr;
            FPoint3D pos;
            Mat4 rotation;

    };

    class Renderer {
        public:
            virtual void init() = 0;
            virtual void render() = 0;
            virtual ~Renderer() {
            }
    };
    class GL3Renderer : public Renderer {
        private:
            enum Flags {
                DRAW_AXIS = 1 << 1
            };
            UINT flags = 0;

            MatrixStack matrix;
            Camera cam;

        public:
            void init() {
                axis = Primitives::genAxis(17);

                cam.pos[1] += 3.f;
                matrix.setCameraCoords(cam);
            }
            void render() {
                if (axis != nullptr)
                    axis->draw(matrix, GL_LINES);
            }

        private:
            Shape* axis = nullptr;
    };
}
namespace Window {
    using namespace Tools;
    using namespace Engine;

    class Window {
        public:
            enum Flags {
                STOP = 0,
                RUNNING = 1 << 0
            };

        private:
            UINT flags = Flags::RUNNING;

            SDL_Window* window;
            IPoint2D bounds;
            Renderer* renderer = nullptr;

            Graphics::Shape* vbo;

        public:
            Window(const IPoint2D& _bounds)
                    :
                      bounds(_bounds) {
                if (!initialize())
                    throw "Nie mogłem otworzyć okna!";
            }

            void open() {
                run();
            }
            void setRenderer(Renderer* renderer) {
                this->renderer = renderer;
            }
            Renderer* getRenderer() {
                return renderer;
            }

            ~Window() {
                SDL_DestroyWindow(window);
                SDL_Quit();
            }

        protected:
            bool initialize() noexcept {
                SDL_Init(SDL_INIT_VIDEO);
                window = SDL_CreateWindow(
                        "Debug",
                        SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED,
                        bounds.X,
                        bounds.Y,
                        SDL_WINDOW_OPENGL
                        );

                return window != nullptr;
            }
            void initContext() {
                glewExperimental = GL_TRUE;
                glewInit();

                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

                SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
                SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LESS);
            }
            void run() {
                SDL_GLContext gl = SDL_GL_CreateContext(window);
                SDL_Event event;

                initContext();
                renderer->init();

                while (IS_SET_FLAG(flags, Flags::RUNNING)) {
                    while (SDL_PollEvent(&event))
                        switch (event.type) {
                            case SDL_KEYDOWN:
                                switch (event.key.keysym.sym) {
                                    case SDLK_LEFT:
                                        break;

                                    case SDLK_RIGHT:
                                        break;

                                    case SDLK_UP:
                                        break;

                                    case SDLK_DOWN:
                                        break;
                                }
                                break;

                            case SDL_QUIT:
                                flags = Flags::STOP;
                                break;
                        }
                    glClearColor(0, 0, 0, 1);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    if (renderer != nullptr)
                        renderer->render();

                    SDL_GL_SwapWindow(window);
                }
                SDL_GL_DeleteContext(gl);
            }
    };
}
int main() {
    try {
        Window::Window wnd( { 400, 400 });
        boost::scoped_ptr<Engine::GL3Renderer> renderer(
                new Engine::GL3Renderer());
        wnd.setRenderer(renderer.get());
        wnd.open();
    } catch (const string& ex) {
        cout << ex;
    }
    return 0;
}
