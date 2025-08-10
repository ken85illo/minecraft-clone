#include "Core/Application.hpp"

int main() {
    if(!glfwInit()) {
        std::println("Failed to initialize GLFW!");
        return -1;
    }

    Application app;
    app.mainLoop();
}
