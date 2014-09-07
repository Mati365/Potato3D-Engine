#include <boost/algorithm/string/erase.hpp>

#include "Config.hpp"
#include "../IO.hpp"

namespace GL3Engine {
    using std::vector;
    using std::string;

    /** Dostep: Header/ID i zwraca wartosc */
    void INIParser::load(c_str& path) {
        vector<string> lines;
        string header;
        data.clear();
        
        IO::getFileContents(path, lines);
        for (string& str : lines)
            if (str[0] == '[')
                header = str.substr(1, str.find(']') - 1);
            else if (str[0] != ';') {
                size_t s = str.find('=');
                string v = str.substr(s + 1);
                
                if (v.find_first_of('"') != string::npos)
                    v = v.substr(v.find_first_of('"') + 1,
                            v.find_last_of('"') - 1);
                else
                    boost::erase_all(v, " ");
                data[header + '/' + str.substr(0, s)] = v;
            }
    }
    INIHeader INIParser::getKeys(c_str header) {
        INIHeader buf;
        for (auto& iter : data) {
            size_t s = iter.first.find('/');
            if (iter.first.substr(0, s) == header)
                buf[iter.first.substr(s + 1)] = iter.second;
        }
        return buf;
    }
}
