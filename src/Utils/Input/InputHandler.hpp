#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>

struct MousePosition {
    double x;
    double y;
};

struct MouseScroll {
    double x;
    double y;
};

class InputHandler {
public:
    static bool isKeyPressed(uint16_t key);
    static bool isMousePressed(uint8_t key);
    static bool isMouseHeld(uint16_t key);
    static bool isKeyHeld(uint16_t key);
    static const MousePosition& getMousePosition();
    static const MouseScroll& getMouseScroll();

    // Callback functions
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
    static bool keyButton[GLFW_KEY_LAST + 1];
    static bool mouseButton[GLFW_MOUSE_BUTTON_LAST + 1];
    static MousePosition mousePosition;
    static MouseScroll mouseScroll;
};
