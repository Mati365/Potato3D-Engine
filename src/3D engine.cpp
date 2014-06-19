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

using namespace std;

namespace Tools {
	typedef unsigned short int UINT;

#define IS_SET_FLAG(num, flag) num & flag

	template<typename T> class Vec2D {
		public:
			T X, Y;

			Vec2D(const T& _X, const T& _Y)
					:
						X(_X),
						Y(_Y) {
			}
			Vec2D& operator+=(const Vec2D& v) {
				X += v.X;
				Y += v.Y;
				return *this;
			}
			Vec2D& operator*(const Vec2D& v) {
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

			virtual ~Vec2D() {
			}
	};
	template<typename T> class Vec3D : public Vec2D<T> {
		public:
			T Z;

			Vec3D(const T& _X, const T& _Y, const T& _Z)
					:
						Vec2D<T>(_X, _Y),
						Z(_Z) {
			}
			Vec3D& operator+=(const Vec3D& v) {
				Vec2D<T>::operator +=(static_cast<Vec2D<T>>(v));
				Z += v.Z;
				return *this;
			}
			Vec3D& operator*(const Vec2D<T>& v) {
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
	template<typename T> class Rect : public Vec2D<T> {
		public:
			T W, H;

			Rect(const T& _X, const T& _Y, const T& _W, const T& _H)
					:
						Vec2D<T>(_X, _Y),
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

	struct ByteColor {
			char R, G, B, A;
	};
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
			template<UINT _rows = rows, UINT _cols = cols>
			Matrix<rows, cols>& operator+=(const Matrix<_rows, _cols>& matrix) {
				for (UINT i = 0; i < rows; ++i)
					for (UINT j = 0, index = 0; j < cols;
							++j, index = i * cols + j)
						this->matrix[index] += matrix.matrix[index];
				return *this;
			}

			GLfloat& operator[](UINT i) {
				return matrix[i];
			}
			GLint getLength() {
				return sizeof(matrix) / sizeof(GLfloat);
			}
	};
	class MatMatrix {
		public:
			/** Rotacja macierzy [ x, y, z, w ] razy kąt */
			static void translate(Matrix<1, 4>& matrix,
					const Vec3D<GLfloat>& v) {
				matrix *= Matrix<4, 4>( {
						1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						v.X, v.Y, v.Z, 1
				});
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

		private:
			void linkShader(initializer_list<GLint> shaders) {
				program = glCreateProgram();
				for (GLint shader : shaders)
					if (shader != 0)
						glAttachShader(program, shader);
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

		glShaderSource(shader, 1, (const char **) allocString(source), &length);
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE) {
			glDeleteShader(shader);
			return 0;
		}
		return shader;
	}
}
namespace Window {
	using namespace Tools;

	class Window {
		private:
			SDL_Window* window;
			Vec2D<int> bounds;

			enum Flags {
				STOP = 0,
				RUNNING = 1 << 0
			};
			UINT flags = Flags::RUNNING;

		public:
			Window(const Vec2D<int>& _bounds)
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
			}
			void run() {
				SDL_GLContext gl = SDL_GL_CreateContext(window);
				SDL_Event event;

				initContext();
				while (IS_SET_FLAG(flags, Flags::RUNNING)) {
					while (SDL_PollEvent(&event))
						switch (event.type) {
							case SDL_QUIT:
								flags = Flags::STOP;
								break;
						}
					glClearColor(0, 0, 0, 1);
					glClear(GL_COLOR_BUFFER_BIT);

					SDL_GL_SwapWindow(window);
				}
				SDL_GL_DeleteContext(gl);
			}
	};
}
int main() {
	try {
		Graphics::Matrix<1, 4> d = {
				-2, -3, 1, 1
		};
		Graphics::MatMatrix::translate(d, { 2, 13, 3 });
		cout << d[1] << endl;

		Window::Window wnd( { 400, 400 });
	} catch (const string& ex) {
		cout << ex;
	}
	return 0;
}
