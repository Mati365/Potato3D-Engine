#include "Game/Game.hpp"

using namespace std;
using namespace GL3Engine;

int main() {
    try {
        Window wnd( { 480, 360 });
        unique_ptr<Game::GameScreen> renderer(
                new Game::GameScreen(&wnd));
        wnd.setRenderer(renderer.get());
        wnd.open();
    } catch (const string& ex) {
        cout << ex;
    }
    return 0;
}
