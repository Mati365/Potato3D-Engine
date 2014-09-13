#include "Resources.hpp"

namespace GL3Engine {
    namespace Resources {
        template<typename C> C* ResourceManager<C>::getResource(c_str path) {
            if (IS_IN_MAP(resources, path))
                return resources[path].get();

            std::string ext = path.substr(path.find('.') + 1);
            if (!IS_IN_MAP(loaders, ext))
                LOG(ERROR, "Unsupported file type!");
            return registerResource(path, loaders[ext]->load(path));
        }
        template<typename C> C* ResourceManager<C>::registerResource(
                c_str handle, C* resource) {
            resources[handle] = std::unique_ptr<C>(resource);
            return resource;
        }
        
        template class ResourceManager<CoreMaterial::Texture> ;
        template class ResourceManager<SceneObject::Shape3D> ;
        template class ResourceManager<CoreEffect::Shader> ;
    }
}
