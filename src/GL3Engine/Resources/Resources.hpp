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
    
    template<typename C>
    class ResourceManager {
        private:
            map<string, unique_ptr<Loader<C> > > loaders;
            map<string, unique_ptr<C>> resources;

        public:
            inline void putLoader(Loader<C>* loader, const string& extension) {
                loaders[extension] = unique_ptr<Loader<C>>(loader);
            }
            C* getResource(c_str);

        private:
            C* registerResource(c_str, C*);
    };
    class GlobalResourceManager :
                                  public Singleton<GlobalResourceManager> {
        private:
            ResourceManager<Texture> textures;
            ResourceManager<Shape3D> shapes;
            ResourceManager<Shader> shaders;

        public:
            GlobalResourceManager();

            template<typename Type>
            Type* getResource(c_str) {
                return nullptr;
            }
            template<typename Type>
            void registerExtension(Loader<Type>*, c_str) {
            }
    };

/** Domyślne typy wczytywane pierwsze */
#define FONT_TEXTURE        "sprites/font.png"
#define DEFAULT_MESH_SHADER "shaders/mesh_shader.glsl"
#define DEFAULT_TEXT_SHADER "shaders/text_shader.glsl"
#define DEFAULT_FBO_SHADER  "shaders/fbo_shader.glsl"

#define REQUIRE_RES(type, handle) \
        GlobalResourceManager::getInstance().getResource<type>(handle)
}

#endif
