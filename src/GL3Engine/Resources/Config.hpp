#ifndef CONFIG_HPP_
#define CONFIG_HPP_
#include "Resources.hpp"

namespace GL3Engine {
    class INIParser {
        private:
            map<string, string> data;

        public:
            INIParser(c_str& path) {
                load(path);
            }

        private:
            void load(c_str&);
    };
}

#endif
