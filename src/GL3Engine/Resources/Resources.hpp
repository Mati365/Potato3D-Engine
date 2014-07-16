#ifndef RESOURCES_HPP_
#define RESOURCES_HPP_
#include "../Graphics/Mesh.hpp"

namespace GL3Engine {
    template<typename C>
    class Loader {
        public:
            virtual C* load(c_str&) = 0;
            virtual ~Loader() {
            }
    };

    using ResourceHandle = GLuint;

    template<typename C>
    class ResourceManager {
        private:
            map<string, unique_ptr<Loader<C> > > loaders;
            vector<unique_ptr<C>> resources;

        public:
            C* getResource(GLuint handle) {
                return resources[handle].get();
            }

            inline void putLoader(Loader<C>* loader,
                    const string& extension) {
                loaders[extension] = unique_ptr<Loader<C>>(loader);
            }

            C* load(c_str&, ResourceHandle* handle = nullptr)
                    throw (string);
            C* registerResource(C*, ResourceHandle*);
    };
    class GlobalResourceManager : public Singleton<GlobalResourceManager> {
        private:
            ResourceManager<Texture> textures;
            ResourceManager<Shape3D> shapes;
            ResourceManager<Shader> shaders;

        public:
            GlobalResourceManager();

            template<typename Type>
            Type* loadResource(c_str&, ResourceHandle* handle = nullptr) {
                return nullptr;
            }
            template<typename Type>
            Type* getResource(ResourceHandle) {
                return nullptr;
            }
            template<typename Type>
            void registerExtension(Loader<Type>*, c_str&) {
            }
    };
    /** Zasoby wymaganie do pracy silnika */
    class RequiredResources : public Singleton<RequiredResources> {
        public:
            struct Package {
                    enum Type {
                        SHADER,
                        MESH,
                        TEXTURE
                    };
                    Type type;
                    string path;
                    string handle;
            };
            map<string, ResourceHandle> packages;

        public:
            RequiredResources();

            void init();
    };

#define DEFAULT_MESH_SHADER "DEFAULT_MESH_SHADER"
#define DEFAULT_TEXT_SHADER "DEFAULT_TEXT_SHADER"
#define REQUIRE_SHADER(handle) \
        GlobalResourceManager::getInstance().getResource<Shader>(RequiredResources::getInstance().packages[handle])
}

#endif
