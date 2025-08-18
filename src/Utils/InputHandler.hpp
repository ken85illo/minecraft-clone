#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <unordered_map>

class InputHandler {
public:
    static bool isKeyPressed(uint16_t key);
    static bool isMousePressed(uint8_t key);
    static bool isMouseHeld(uint16_t key);
    static bool isKeyHeld(uint16_t key);
    static const glm::dvec2 &getMousePosition();
    static const glm::dvec2 &getMouseScroll();

    // Callback functions
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    static void cursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

private:
    static bool s_keyButton[GLFW_KEY_LAST + 1];
    static bool s_mouseButton[GLFW_MOUSE_BUTTON_LAST + 1];
    static glm::dvec2 s_mousePosition;
    static glm::dvec2 s_mouseScroll;
};
