#include "ECS.hpp"

namespace GL3Engine {
    class Test : public Component {
        public:
            Test() {
                cout << typeid(Component).name() << endl;
            }
    };

    class Point : public Component {
        RTII_REG_CLASS(Point)

        public:
            GLfloat x = 2, y;
    };
    DECLARE_CTYPE(Point)

    class Rect : public Component {
        RTII_REG_CLASS(Rect)

        public:
            GLfloat x = 4, y = 5, w = 10, h = 15;
    };
    DECLARE_CTYPE(Rect)

    class PointSystem : public CTSystem<Point> {
            virtual void logic(Entity* c) {
                cout << c->get<Point>()->x << endl;
                c->get<Point>()->x += 2;
            }
    };
    class RectSystem : public CTSystem<Rect> {
            virtual void logic(Entity* c) {
                cout << "E" << c->get<Point>()->x << endl;
                c->get<Point>()->x += 2;
            }
    };

    void debugECS() {
        CWorld world;
        world.regSystem<PointSystem>();
        world.regSystem<RectSystem>();

        Entity* dick = new Entity();
        dick->add(REQUIRE_CTYPE_OBJ("Rect"));
        dick->add(REQUIRE_CTYPE_OBJ("Point"));

        world.regEntity(dick);
        world.update();
    }
}
