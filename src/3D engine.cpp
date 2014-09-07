#include "Game/Game.hpp"

using namespace GL3Engine;

int main() {
    try {
        CoreWindow::Window wnd( { 512, 256 });
        std::unique_ptr<Game::GameScreen> renderer(new Game::GameScreen(&wnd));
        wnd.setRenderer(renderer.get());
        wnd.open();
    } catch (c_str ex) {
        LOG(ERROR, ex);
    }
    return 0;
}
