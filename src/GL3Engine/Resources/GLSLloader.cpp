#include "Loaders.hpp"

namespace GL3Engine {
    namespace CoreLoader {
        TYPE_IMPORT(CoreEffect, Shader);
        TYPE_IMPORT(IO, getFileContents);
        TYPE_IMPORT(std, string);

        string GLSLloader::putToFileName(string path, c_str prefix) {
            size_t pos = path.find('/');
            path.insert(pos + 1, prefix);
            return path;
        }
        Shader* GLSLloader::load(c_str path) {
            return new Shader(
                    getFileContents(putToFileName(path, "frag_")),
                    getFileContents(putToFileName(path, "vert_")),
                    getFileContents(putToFileName(path, "geo_")));
        }
    }
}
