#ifndef MAP_HPP_
#define MAP_HPP_
#include "../GL3Engine/Window/Window.hpp"

namespace Game {
    using namespace GL3Engine;

    struct MapNode {
            enum Flag {
                COLLISION = 1 << 0,
            };
    };
    class Map {
        public:

    };
}

#endif
