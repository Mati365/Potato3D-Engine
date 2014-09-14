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

#include "StockUtils.hpp"

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

    namespace CoreInterface {
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
        class NonCopyable {
            public:
                NonCopyable() {
                }
                virtual ~NonCopyable() {
                }
            private:
                NonCopyable& operator=(const NonCopyable&) {
                    return *this;
                }
                NonCopyable(const NonCopyable&) {
                }
        };

        template<typename T> class ContainerManager :
                                                      public NonCopyable,
                                                      public Singleton<
                                                              ContainerManager<T>> {
            private:
                std::vector<T> objects;

            public:
                GLint regObject(T& obj) {
                    objects.push_back(obj);
                    return objects.size() - 1;
                }
                T& operator[](GLint key) {
                    return objects[key];
                }
        };
    }
}
namespace Tools {
    constexpr size_t constHash(const GLchar* input) {
        return *input ?
                        static_cast<size_t>(*input)
                                + 33 * constHash(input + 1) :
                        5381;
    }
    extern size_t hashString(c_str);
    extern std::vector<std::string> tokenize(c_str, char);
    
    /** Konwersje */
    template<typename T> T stringTo(c_str str) {
        std::istringstream iss(str);
        T num;
        iss >> num;
        return num;
    }
    template<typename T> std::string toString(const T& num) {
        std::ostringstream ss;
        ss << num;
        return ss.str();
    }
    
    template<typename T> T toRad(const T& v) {
        return v * 180.f / 3.145f;
    }
    template<typename T, GLint size> inline void copyToRaw(
            const std::array<T, size>& source, T* destination) {
        std::copy(source.begin(), source.end(), destination);
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
    
    struct Log {
            enum class Flag {
                CRITICAL,
                WARNING,
                ERROR
            };

            Flag flag;
            std::string message;

            static std::deque<Log> logs;
            static void putLog(Flag, c_str);
    };
#define LOG(type, str) Tools::Log::putLog(Tools::Log::Flag::type, str)
    
    extern void showGLErrors();
}

#endif
