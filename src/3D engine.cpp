#include <iostream>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <math.h>
#include <deque>
#include <initializer_list>

#include <sstream>
#include <fstream>
#include <stdio.h>

#include <boost/scoped_ptr.hpp>

using namespace std;

namespace Tools {
	typedef unsigned short int UINT;

#define IS_SET_FLAG(num, flag) num & flag

	template<typename T> class Point2D {
		public:
			T X, Y;

			Point2D(const T& _X, const T& _Y)
					:
						X(_X),
						Y(_Y) {
			}
			Point2D& operator+=(const Point2D& v) {
				X += v.X;
				Y += v.Y;
				return *this;
			}
			Point2D& operator*(const Point2D& v) {
				X *= v.X;
				Y *= v.Y;
				return *this;
			}
			virtual inline T getVecLength() const {
				return sqrt(pow(X, 2) + pow(Y, 2));
			}
			virtual void normalize() {
				T length = getVecLength();
				this->X /= length;
				this->Y /= length;
			}

			virtual ~Point2D() {
			}
	};
	template<typename T> class Point3D : public Point2D<T> {
		public:
			T Z;

			Point3D(const T& _X, const T& _Y, const T& _Z)
					:
						Point2D<T>(_X, _Y),
						Z(_Z) {
			}
			Point3D& operator+=(const Point3D& v) {
				Point2D<T>::operator +=(static_cast<Point2D<T>>(v));
				Z += v.Z;
				return *this;
			}
			Point3D& operator*(const Point2D<T>& v) {
				this->X *= v.X;
				this->Y *= v.Y;
				return *this;
			}
			inline T getVecLength() const {
				return sqrt(pow(this->X, 2) + pow(this->Y, 2) + pow(this->Z, 2));
			}
			void normalize() {
				T length = getVecLength();
				this->X /= length;
				this->Y /= length;
				this->Z /= length;
			}
	};
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

	template<class T> inline UINT getArrayLength(T* ptr) {
		return sizeof(ptr) / sizeof(T);
	}
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

	struct ByteColor {
			char R, G, B, A;
	};

	/**
	 * Specjalizacje szablonu są
	 * szybsze nieżeli ciągłe
	 * kasowanie i tworzenie
	 * dynamicznie alokowanych
	 * tablic w tymczasowych macierzach
	 */
	template<UINT rows, UINT cols> struct Matrix {
			static const UINT ROWS = rows, COLS = cols;
			GLfloat matrix[rows * cols];

			Matrix() {
				memset(&matrix, 0, sizeof(matrix));
			}
			Matrix(initializer_list<GLfloat> array) {
				for (UINT i = 0; i < array.size(); ++i)
					matrix[i] = *(array.begin() + i);
			}

			/** Operacje na macierzach */
			Matrix<rows, cols>& operator*=(GLfloat k) {
				for (GLfloat& a : matrix)
					a *= k;
				return *this;
			}
			template<UINT _rows, UINT _cols>
			Matrix<rows, cols>& operator*=(const Matrix<_rows, _cols>& matrix) {
				GLfloat temp[rows * cols];
				for (UINT i = 0; i < rows; ++i)
					for (UINT j = 0; j < cols; ++j) {
						GLfloat sum = 0.f;
						for (UINT k = 0; k < cols; ++k)
							sum += (*this)[i * cols + k]
									* matrix.matrix[k * _cols + j];
						temp[i * cols + j] = sum;
					}
				memcpy(this->matrix, temp, sizeof(temp));
				return *this;
			}
			template<UINT _rows, UINT _cols>
			Matrix<rows, cols>& operator+=(
					const Matrix<_rows, _cols>& matrix) {
				for (UINT i = 0; i < rows; ++i)
					for (UINT j = 0, index = 0; j < cols;
							++j, index = i * cols + j)
						this->matrix[index] += matrix.matrix[index];
				return *this;
			}
			template<UINT _rows, UINT _cols>
			inline Matrix<rows, cols>& operator=(
					const Matrix<_rows, cols>& matrix) {
				memcpy(this->matrix, matrix.matrix, sizeof(this->matrix));
				return *this;
			}

			GLfloat& operator[](UINT i) {
				return matrix[i];
			}
			inline GLint getLength() const {
				return getArrayLength(matrix);
			}
	};
	using Mat4 = Matrix<4, 4>;
	using Mat3 = Matrix<3, 3>;
	using Mat2 = Matrix<2, 2>;

	using Vec4 = Matrix<4, 1>;
	using Vec3 = Matrix<3, 1>;
	using Vec2 = Matrix<2, 1>;

	/**
	 * Żeby nie pakować wszystkiego
	 * do matrixa to jest oddzielona
	 * klasa do przekształceń
	 */
	template<UINT rows> class MatMatrix {
		public:
			/** Operacje na macierzy macierzy [ x, y, z, w ] */
			static Mat4 identity() {
				return Mat4( {
						1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1
				});
			}
			static inline void identity(Matrix<rows, 4>& matrix) {
				matrix = identity();
			}

			static Mat4 translate(const Point3D<GLfloat>& v) {
				return Mat4( {
						1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						v.X, v.Y, v.Z, 1
				});
			}
			static inline void translate(Matrix<rows, 4>& matrix,
					const Point3D<GLfloat>& v) {
				matrix *= translate(v);
			}

			static Mat4 scale(const Point3D<GLfloat>& scale) {
				return Mat4( {
						scale.X, 0, 0, 0,
						0, scale.Y, 0, 0,
						0, 0, scale.Z, 0,
						0, 0, 0, 1
				});
			}
			static inline void scale(Matrix<rows, 4>& matrix,
					const Point3D<GLfloat>& _scale) {
				matrix *= scale(_scale);
			}

			static Mat4 rotate(GLfloat theta, const Point3D<GLfloat>& axis) {
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
			static inline void rotate(Matrix<rows, 4>& matrix, GLfloat theta,
					const Point3D<GLfloat>& axis) {
				matrix *= rotate(theta, axis);
			}
	};

	/**
	 * Rendering brył
	 * Atrybut:
	 * 0 = in_Position
	 * 1 = in_Color
	 */
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
			GLfloat x, y, z;
			GLfloat r, g, b, a;
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
				shader.begin();

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
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
						sizeof(Vertex), 0);
				glEnableVertexAttribArray(0);

				// Color
				glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
						sizeof(Vertex),
						BUFFER_OFFSET(3 * sizeof(GLfloat)));
				glEnableVertexAttribArray(1);
				glBindVertexArray(0);
			}
	};
}
namespace Window {
	using namespace Tools;

	class Window {
		private:
			SDL_Window* window;
			Point2D<int> bounds;

			enum Flags {
				STOP = 0,
				RUNNING = 1 << 0
			};
			UINT flags = Flags::RUNNING;

			Graphics::Shape* vbo;

		public:
			Window(const Point2D<int>& _bounds)
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
			}
			void run() {
				SDL_GLContext gl = SDL_GL_CreateContext(window);
				SDL_Event event;

				initContext();

				Graphics::Vertex p[] = {
						{ -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.f },
						{ 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.f },
						{ 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.f } };
				vbo = new Graphics::Shape(p, 21);
				while (IS_SET_FLAG(flags, Flags::RUNNING)) {
					while (SDL_PollEvent(&event))
						switch (event.type) {
							case SDL_QUIT:
								flags = Flags::STOP;
								break;
						}
					glClearColor(0, 0, 0, 1);
					glClear(GL_COLOR_BUFFER_BIT);

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
