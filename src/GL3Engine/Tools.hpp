#ifndef TOOLS_HPP_
#define TOOLS_HPP_
#include <deque>
#include <vector>
#include <array>

#include <GL/gl.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <assert.h>
#include <functional>

#include <type_traits>

namespace std {
    template<typename T> inline T* begin(const pair<T*, T*>& p)
            {
        return p.first;
    }
    template<typename T> inline T* end(const pair<T*, T*>& p)
            {
        return p.second;
    }
}
namespace GL3Engine {
    GLuint operator*(GLuint, std::function<void(void)>);
}
namespace Tools {
    using namespace std;
    
    using c_str = const string&;
    using GLuchar = unsigned char;
    
#define FORCE_INLINE __attribute__((always_inline))
#define REMOVE_FLAG(num, flag) ((num) & ~(flag))
#define IS_SET_FLAG(num, flag) ((num) & (flag))
    
#define ARRAY_LENGTH(type, array) (sizeof(array) / sizeof(type))
#define IS_IN_MAP(map, key) (map.find(key) != map.cend())
    
    constexpr GLint constHash(const GLchar* input) {
        return *input ?
                        static_cast<GLuint>(*input)
                                + 33 * constHash(input + 1) :
                        5381;
    }
    extern size_t hashString(c_str);
    extern vector<string> tokenize(c_str, char);
    
    /** Konwersje */
    template<typename T> T stringTo(c_str str) {
        T num;
        istringstream iss(str);
        iss >> num;
        return num;
    }
    template<typename T> string toString(const T& num) {
        ostringstream ss;
        ss << num;
        return ss.str();
    }
    
    template<typename T> T toRad(const T& v) {
        return v * 180.f / 3.145f;
    }
    template<typename T, GLint size> inline void copyToRaw(
            const array<T, size>& source, T* destination) {
        copy(source.begin(), source.end(), destination);
    }
    
    template<typename T> void safeDelete(T*& ptr, GLboolean arr) {
        if (!ptr)
            return;
        
        if (arr)
            delete[] ptr;
        else
            delete ptr;
        ptr = nullptr;
    }
    template<typename T, typename E> GLuint count(T* array, E el) {
        GLuint buf;
        for (T* ptr = &array[0]; *ptr; ptr++)
            if (*ptr == el)
                buf++;
        return buf;
    }
    template<typename T, typename E> GLboolean instanceOf(T* obj) {
        return dynamic_cast<E*>(&obj);
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
            static void putLog(Flag, const string&);
    };
#define LOG(type, str) Log::putLog(Log::type, str)
    
    extern void showGLErrors();
    
    template<typename T> class MemAlloc {
        public:
            virtual T* createObject() = 0;
            virtual void releaseMemory() = 0;
            virtual ~MemAlloc() {
            }
    };
    template<typename T> class Singleton {
        protected:
            Singleton() {
            }
            
        public:
            static inline T& getInstance() {
                static T t;
                return t;
            }
    };
}

#endif
