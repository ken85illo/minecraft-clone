#include "Engine.hpp"

int main() {
    if(!glfwInit()) {
        std::println("Failed to initialize GLFW!");
        return -1;
    }

    Engine engine;
    engine.mainLoop();
}
