#include "Resources.hpp"

namespace GL3Engine {
    template<typename C>
    C* ResourceManager<C>::getResource(c_str path) {
        if (IS_IN_MAP(resources, path))
            return resources[path].get();

        string ext = path.substr(path.find('.') + 1);
        if (!IS_IN_MAP(loaders, ext))
            LOG(ERROR, "Unsupported file type!");
        return registerResource(path, loaders[ext]->load(path));
    }
    template<typename C>
    C* ResourceManager<C>::registerResource(c_str handle, C* resource) {
        resources[handle] = unique_ptr<C>(resource);
        return resource;
    }

    template class ResourceManager<Texture> ;
    template class ResourceManager<Shape3D> ;
    template class ResourceManager<Shader> ;
}
