#include "Engine.hpp"
#include <Window.hpp>


int main() {
    if(!glfwInit()) {
        std::println("Failed to initialize GLFW!");
        return -1;
    }

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    Engine::windowWidth = mode->width;
    Engine::windowHeight = mode->height;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    Window::instance = new Window("ain't no way bruh", monitor);
    Window::instance->mainLoop();

    if(Engine::worldShader)
        delete Engine::worldShader;

    if(Engine::lineShader)
        delete Engine::lineShader;

    delete Window::instance;
}
