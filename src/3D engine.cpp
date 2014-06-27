#include "GL3Engine/Window/Window.hpp"

using namespace std;
using namespace GL3Engine;

int main() {
    try {
        Window wnd( { 400, 400 });
        unique_ptr<GL3Renderer> renderer(
                new GL3Renderer());
        wnd.setRenderer(renderer.get());
        wnd.open();
    } catch (const string& ex) {
        cout << ex;
    }
    return 0;
}
