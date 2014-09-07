#include "Scene.hpp"

namespace GL3Engine {
    namespace CoreEffect {
        GLfloat& EffectManager::EffectParam::operator[](GLuint index) {
            if (data.size() <= index)
                data.resize(index + 1);
            return data[index];
        }
        EffectManager::EffectParam& EffectManager::setEffectParam(c_str param,
                GLuint type) {
            EffectParam& p = effect_params[param];
            p.type = type;
            return p;
        }

        void EffectManager::begin() {
            if (!attrib)
                return;
            attrib->begin();
            for (auto& el : effect_params)
                attrib->setUniform(el.first, &el.second.data[0],
                        el.second.data.size(), el.second.type);
        }
        void EffectManager::end() {
            if (attrib)
                attrib->end();
        }
    }
}

