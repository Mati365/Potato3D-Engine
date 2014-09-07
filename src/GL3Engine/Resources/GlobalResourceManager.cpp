#include <vector>
#include <algorithm>
#include <sstream>

#include "Loaders.hpp"
#include "Config.hpp"

namespace GL3Engine {
    namespace Resources {
        using CoreInterface::Loader;
        using std::string;

#define DECLARE_EXTENSION(stack, type) \
        template<> \
        void GlobalResourceManager::regExtension(Loader<type>* l, c_str e) { \
            stack.putLoader(l, e); \
        } \
        template<> \
        type* GlobalResourceManager::getResource(c_str handle) { \
            return stack.getResource(handle); \
        }

        DECLARE_EXTENSION(textures, CoreMaterial::Texture);
        DECLARE_EXTENSION(shapes, SceneObject::Shape3D);
        DECLARE_EXTENSION(shaders, CoreEffect::Shader);

        // unique_ptr w klasie ResourceManager
        GlobalResourceManager::GlobalResourceManager() {
            regExtension(new MD5::MD5loader, "md5mesh");
            regExtension(new OBJ::OBJloader, "obj");
            regExtension(new CoreLoader::GLSLloader, "glsl");
            regExtension(new CoreLoader::Textureloader, "png");
        }
    }
}

