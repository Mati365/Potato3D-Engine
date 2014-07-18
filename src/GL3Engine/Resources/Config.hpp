#ifndef CONFIG_HPP_
#define CONFIG_HPP_
#include "Resources.hpp"

namespace GL3Engine {
    using INIHeader = map<string, string>;
    class INIParser {
        private:
            INIHeader data;

        public:
            INIParser(c_str& path) {
                load(path);
            }
            string& getValue(c_str header, c_str key) {
                return data[header + "/" + key];
            }
            INIHeader getKeys(c_str);

        private:
            void load(c_str&);
    };
}

#endif
