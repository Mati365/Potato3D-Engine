#ifndef RESOURCES_HPP_
#define RESOURCES_HPP_
#include "../Graphics/Mesh.hpp"

namespace GL3Engine {
    namespace CoreInterface {
        template<typename C> class Loader {
            public:
                virtual C* load(c_str&) = 0;
                virtual ~Loader() {
                }
        };
    }
    namespace Resources {
        template<typename C> class ResourceManager {
            private:
                std::map<std::string, std::unique_ptr<CoreInterface::Loader<C> > > loaders;
                std::map<std::string, std::unique_ptr<C>> resources;

            public:
                inline void putLoader(CoreInterface::Loader<C>* loader,
                        c_str extension) {
                    loaders[extension] =
                            std::unique_ptr<CoreInterface::Loader<C>>(loader);
                }
                C* getResource(c_str);

            private:
                C* registerResource(c_str, C*);
        };
        class GlobalResourceManager :
                                      public CoreInterface::Singleton<
                                              GlobalResourceManager> {
            private:
                ResourceManager<CoreMaterial::Texture> textures;
                ResourceManager<SceneObject::Shape3D> shapes;
                ResourceManager<CoreEffect::Shader> shaders;

            public:
                GlobalResourceManager();

                template<typename Type> Type* getResource(c_str) {
                    return nullptr;
                }
                template<typename Type> void regExtension(
                        CoreInterface::Loader<Type>*, c_str) {
                }
        };
    }

/** Domyślne typy wczytywane pierwsze */
#define FONT_TEXTURE            "sprites/font.png"
#define DEFAULT_MESH_SHADER     "shaders/mesh_shader.glsl"
#define DEFAULT_TEXT_SHADER     "shaders/text_shader.glsl"
#define DEFAULT_FBO_SHADER      "shaders/fbo_shader.glsl"
#define DEFAULT_SHADOW_SHADER   "shaders/shadow_mesh_shader.glsl"

#define REQUIRE_RES(type, handle) \
        GL3Engine::Resources::GlobalResourceManager::getInstance().getResource<type>(handle)
}

#endif
