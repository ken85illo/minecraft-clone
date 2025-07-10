#include <Window.hpp>

int main() {
    Window::instance = new Window(1280, 800, "Hello OpenGL!");
    Window::instance->mainLoop();
    delete Window::instance;
}
