#include <vector>
#include <algorithm>
#include <sstream>

#include "Loaders.hpp"
#include "Config.hpp"

namespace GL3Engine {
#define DECLARE_EXTENSION(stack, type) \
    template<> \
    void GlobalResourceManager::registerExtension(Loader<type>* l, c_str e) { \
        stack.putLoader(l, e); \
    } \
    template<> \
    type* GlobalResourceManager::getResource(c_str handle) { \
        return stack.getResource(handle); \
    }
    
    DECLARE_EXTENSION(textures, Texture);
    DECLARE_EXTENSION(shapes, Shape3D);
    DECLARE_EXTENSION(shaders, Shader);
    
    GlobalResourceManager::GlobalResourceManager() {
        registerExtension(new MD5::MD5loader, "md5mesh");
        registerExtension(new OBJ::OBJloader, "obj");
        registerExtension(new GLSLloader, "glsl");
        registerExtension(new Textureloader, "png");
    }
}

