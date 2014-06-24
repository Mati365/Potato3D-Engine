#include <iostream>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <initializer_list>
#include <math.h>
#include <deque>

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
				ERRO
			};

			Flag flag;
			string message;

			static deque<Log> logs;
	};
	deque<Log> Log::logs;

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

			T* get(UINT x, UINT y) {
				return matrix[y * cols + x];
			}
			T* operator[](UINT i) {
				return matrix[i];
			}
			inline GLint getLength() const {
				return rows * cols;
			}

			~Matrix() {
				safe_delete(matrix, true);
			}
	};
	template<typename T> Matrix<T> operator*(const Matrix<T>& l, const Matrix<T>& r) {
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
#define FMAT_MATH Graphics::MatMatrix<GLfloat>
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

#define MAT_STACK MatrixStack::getInstance()
	class MatrixStack : public Singleton<MatrixStack> {
		public:
			Mat4 projection, view, model;

			MatrixStack() {
				projection = FMAT_MATH::perspective(90.f, 4.0 / 3.0, 1.f, 100.f);
				view = FMAT_MATH::lookAt(
						{	3,0,-1},
						{	0,0,0},
						{	0,1,0});
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
			virtual void draw(GLint)=0;
			virtual ~Drawable() {
			}
	};
	struct Vertex {
			GLfloat x, y, z, w, r, g, b, a;
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
			GLuint vao = 0, vbo = 0;

			Shape(Vertex* buffer, GLint len) {
				create(buffer, len);
			}
			void draw(GLint mode) {
				static Shader shader(
						getFileContents("shaders/fragment_shader.txt"),
						getFileContents("shaders/vertex_shader.txt"),
						"");
				static float angle = 0.f;
				angle += 0.0001;

				shader.begin();

				//Matrix<float> mat = FMAT_MATH::rotate(TO_RAD(angle), {0.0, 0.0, 1.0}) * FMAT_MATH::scale({0.5, 0.5, 1.0});
				shader.setUniform("mvp",
				MAT_STACK.projection *
				MAT_STACK.view *
				FMAT_MATH::rotate(angle, {1, 0, 0.0}) *
				FMAT_MATH::scale( {0.5, 0.5, 1.0}));

				glBindVertexArray(vao);
				glDrawArrays(mode, 0, 3);
				glBindVertexArray(0);

				shader.end();
			}

			~Shape() {
				glDeleteBuffers(1, &vbo);
			}
		private:
			void create(Vertex* buffer, GLint len) {
				glGenVertexArrays(1, &vao);
				glBindVertexArray(vao);

				// Generowanie bufora
				vbo = genGLBuffer<Vertex>(
						buffer,
						len,
						GL_ARRAY_BUFFER);

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

				// Vertex
				glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
						sizeof(Vertex), 0);
				glEnableVertexAttribArray(0);

				// Color
				glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
						sizeof(Vertex),
						BUFFER_OFFSET(4 * sizeof(GLfloat)));
				glEnableVertexAttribArray(1);
				glBindVertexArray(0);
			}
	};
}
namespace Engine {
	using namespace Graphics;

	class GlobalRenderer : public Drawable {
		public:
	};
}
namespace Window {
	using namespace Tools;

	class Window {
		private:
			SDL_Window* window;
			IPoint2D bounds;

			enum Flags {
				STOP = 0,
				RUNNING = 1 << 0
			};
			UINT flags = Flags::RUNNING;

			Graphics::Shape* vbo;

		public:
			Window(const IPoint2D& _bounds)
					:
						bounds(_bounds) {
				if (initialize())
					run();
				else
					throw "Nie mogłem otworzyć okna!";
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

				Graphics::Vertex p[] = {
						{ -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.f },
						{ 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.f },
						{ 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.f } };
				vbo = new Graphics::Shape(p, 24);
				while (IS_SET_FLAG(flags, Flags::RUNNING)) {
					while (SDL_PollEvent(&event))
						switch (event.type) {
							case SDL_QUIT:
								flags = Flags::STOP;
								break;
						}
					glClearColor(0, 0, 0, 1);
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

					vbo->draw(GL_TRIANGLES);

					SDL_GL_SwapWindow(window);
				}
				SDL_GL_DeleteContext(gl);
			}
	};
}
int main() {
	try {
		Window::Window wnd( { 400, 400 });
	} catch (const string& ex) {
		cout << ex;
	}
	return 0;
}
