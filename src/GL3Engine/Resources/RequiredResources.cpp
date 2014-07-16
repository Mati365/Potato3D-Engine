#include "Resources.hpp"

namespace GL3Engine {
    vector<RequiredResources::Package> GAME_CONTENT = {
            {
                    RequiredResources::Package::SHADER,
                    "shaders/mesh_shader.glsl",
                    DEFAULT_MESH_SHADER
            },
            {
                    RequiredResources::Package::SHADER,
                    "shaders/text_shader.glsl",
                    DEFAULT_TEXT_SHADER
            }
    };
    RequiredResources::RequiredResources() {
        GlobalResourceManager& res = GlobalResourceManager::getInstance();
        for (Package package : GAME_CONTENT) {
            ResourceHandle res_handle = 0;
            switch (package.type) {
                case Package::SHADER:
                    res.loadResource<Shader>(package.path, &res_handle);
                    break;
                case Package::MESH:
                    break;
                case Package::TEXTURE:
                    break;
                default:
                    return;
            }
            packages[package.handle] = res_handle;
        }
    }
}

