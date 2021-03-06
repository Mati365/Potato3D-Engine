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
    template<typename T> inline T* begin(const pair<T*, T*>& p) {
        return p.first;
    }
    template<typename T> inline T* end(const pair<T*, T*>& p) {
        return p.second;
    }
}
namespace GL3Engine {
    GLuint operator*(GLuint, std::function<void(void)>);

    namespace CoreInterface {
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
        template<typename T> class MemAlloc {
            public:
                virtual T* createObject() = 0;
                virtual void releaseMemory() = 0;
                virtual ~MemAlloc() {
                }
        };
        template<typename T> class Singleton :
                                               public CoreInterface::NonCopyable {
            public:
                static inline T& getInstance() {
                    static T t;
                    return t;
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
}

#endif
