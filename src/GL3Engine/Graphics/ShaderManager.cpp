#include <utility>

#include "Effects.hpp"
#include "../IO.hpp"

namespace GL3Engine {
    using namespace IO;

    ShaderManager::ShaderInfo ShaderManager::DEFAULT_SHADERS[] = {
            {
                    DEFAULT_MESH_SHADER,
                    {
                            "shaders/shape_frag.glsl",
                            "shaders/shape_vert.glsl",
                            ""
                    }
            },
            {
                    DEFAULT_TEXT_SHADER,
                    {
                            "shaders/text_frag.glsl",
                            "shaders/text_vert.glsl",
                            ""
                    }
            }
    };

    void ShaderManager::putShader(ShaderInfo& shader) {
        shaders[shader.handle] = new Shader(
                getFileContents(shader.content[0]),
                getFileContents(shader.content[1]),
                getFileContents(shader.content[2])
                        );
    }
    void ShaderManager::init() {
        for (ShaderInfo& shader : DEFAULT_SHADERS)
            putShader(shader);
    }
    void ShaderManager::destroy() {
        for (pair<GLuint, Shader*> shader : shaders)
            safeDelete(shader.second, false);
    }
}

