#include "Loaders.hpp"

namespace GL3Engine {
    namespace OBJ {
        TYPE_IMPORT(std, string);
        TYPE_IMPORT(std, vector);

        template<typename T> T* ASCIIMeshLoader<T>::load(c_str& path) {
            vector<string> lines;
            string dir = path.substr(0, path.find_last_of('/') + 1);

            IO::getFileContents(path, lines);
            for (string& line : lines) {
                GLint active_header = 0;
                vector<string> tokens = Tools::tokenize(line, ' ');

                if (headers.find(tokens[0]) != headers.end()) {
                    active_header = headers.find(tokens[0])->second;
                    onNewHeader(active_header, tokens);
                } else
                    continue;
                for (auto it = tokens.begin() + 1; it != tokens.end(); ++it)
                    onHeaderArgument(dir, active_header, it);
            }
            
            return selfCreateObject();
        }
        template<typename T> T* ASCIIMeshLoader<T>::selfCreateObject() {
            T* obj = this->createObject();
            this->releaseMemory();
            return obj;
        }

        TYPE_IMPORT(CoreMatrix, Vec2);
        TYPE_IMPORT(CoreMatrix, Vec3);

        template<typename T> Vec3 ASCIIMeshLoader<T>::getVec3D(
                LoaderIterator& iter) {
            Vec3 v;
            sscanf(
                    (*iter + " " + *(iter + 1) + " " + *(iter + 2))
                            .c_str(),
                    "%f %f %f", &v[0], &v[1], &v[2]);
            iter += 2;
            return v;
        }
        template<typename T> Vec2 ASCIIMeshLoader<T>::getVec2D(
                LoaderIterator& iter) {
            Vec2 v;
            sscanf(
                    (*iter + " " + *(iter + 1)).c_str(),
                    "%f %f", &v[0], &v[1]);
            iter++;
            return v;
        }
        
        template class ASCIIMeshLoader<CoreMaterial::Materials> ;
        template class ASCIIMeshLoader<SceneObject::Shape3D> ;
    }
}

