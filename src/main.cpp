#include <Window.hpp>

int main() {
    Window::instance = new Window(1280, 720, "ain't no way bruh");
    Window::instance->mainLoop();
    delete Window::instance;
}
