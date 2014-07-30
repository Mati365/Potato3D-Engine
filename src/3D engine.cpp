#include "Game/Game.hpp"
#include "GL3Engine/ECS/ECS.hpp"

using namespace std;
using namespace GL3Engine;

int main() {
    try {
        if(0) {
        Window wnd( { 480, 320 });
        unique_ptr<Game::GameScreen> renderer(
                new Game::GameScreen(&wnd));
        wnd.setRenderer(renderer.get());
        wnd.open();
        }
        debugECS();
    } catch (const string& ex) {
        cout << ex;
    }
    return 0;
}
