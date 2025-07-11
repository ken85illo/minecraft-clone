#include <Window.hpp>

int main() {
    Window::instance = new Window(960, 720, "Hello OpenGL!");
    Window::instance->mainLoop();
    delete Window::instance;
}
