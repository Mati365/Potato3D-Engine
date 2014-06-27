#ifndef TOOLS_HPP_
#define TOOLS_HPP_
#include <deque>
#include <GL/gl.h>
#include <iostream>

namespace Tools {
    using namespace std;

#define IS_SET_FLAG(num, flag) (num & flag)
#define ARRAY_LENGTH(type, array) (sizeof(array) / sizeof(type))
#define IS_IN_MAP(map, key) (map.find(key) != map.end())

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
