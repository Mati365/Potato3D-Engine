#include "Resources.hpp"

namespace GL3Engine {
    template<typename C>
    C* ResourceManager<C>::load(c_str& path, ResourceHandle* handle)
            throw (string) {
        string ext = path.substr(path.find('.') + 1);
        if (!IS_IN_MAP(loaders, ext))
            throw "Unsupported file type!";
        return registerResource(loaders[ext]->load(path), handle);
    }
    template<typename C>
    C* ResourceManager<C>::registerResource(C* resource,
            ResourceHandle* handle) {
        resources.push_back(unique_ptr<C>(resource));
        if (handle)
            *handle = resources.size() - 1;
        return resource;
    }

    template class ResourceManager<Texture> ;
    template class ResourceManager<Shape3D> ;
    template class ResourceManager<Shader> ;
}
