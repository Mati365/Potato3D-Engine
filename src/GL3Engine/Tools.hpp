#ifndef TOOLS_HPP_
#define TOOLS_HPP_
#include <deque>
#include <vector>
#include <array>

#include <GL/gl.h>
#include <iostream>
#include <sstream>
#include <memory>

namespace Tools {
    using namespace std;

    using c_str = const string&;
    using GLuchar = unsigned char;

#define IS_SET_FLAG(num, flag) ((num) & (flag))
#define ARRAY_LENGTH(type, array) (sizeof(array) / sizeof(type))
#define IS_IN_MAP(map, key) (map.find(key) != map.end())

    extern vector<string> tokenize(const string&, char);

    /** Konwersje */
    template<typename T> T stringTo(const string& str) {
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
        return v * 180 / 3.14;
    }
    template<typename T, GLint size> inline void arrayToRaw(
            const array<T, size>& source, T* destination) {
        copy(source.begin(), source.end(), destination);
    }

    template<typename T> void safeDelete(T*& ptr, bool arr) {
        if (ptr == nullptr)
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
    extern void showGLErrors();

    template<typename T> class Singleton {
        public:
            static T& getInstance() {
                static T t;
                return t;
            }
    };
}

#endif
