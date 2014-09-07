#ifndef CONFIG_HPP_
#define CONFIG_HPP_
#include "Resources.hpp"

namespace GL3Engine {
    using INIHeader = std::map<std::string, std::string>;
    class INIParser {
        private:
            INIHeader data;

        public:
            INIParser(c_str& path) {
                load(path);
            }
            std::string& getValue(c_str header, c_str key) {
                return data[header + "/" + key];
            }
            INIHeader getKeys(c_str);

        private:
            void load(c_str&);
    };
}

#endif
