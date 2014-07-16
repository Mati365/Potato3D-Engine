#include "Loaders.hpp"

namespace GL3Engine {
    template<typename T>
    T* ASCIIMeshLoader<T>::load(c_str& path) {
        vector<string> lines;

        getFileContents(path, lines);
        for (string& line : lines) {
            GLint active_header = 0;
            vector<string> tokens = tokenize(line, ' ');

            if (headers.find(tokens[0]) != headers.end()) {
                active_header = headers.find(tokens[0])->second;
                onNewHeader(active_header, tokens);
            } else
                continue;
            for (auto it = tokens.begin() + 1; it != tokens.end();
                    ++it)
                onHeaderArgument(active_header, it);
        }
        return selfCreateObject();
    }
    template<typename T>
    T* ASCIIMeshLoader<T>::selfCreateObject() {
        T* obj = createObject();
        releaseMemory();
        return obj;
    }

    template class ASCIIMeshLoader<MATERIALS> ;
    template class ASCIIMeshLoader<Shape3D> ;
}

