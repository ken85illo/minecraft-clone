#include "InputHandler.hpp"

bool InputHandler::s_keyButton[GLFW_KEY_LAST + 1];
bool InputHandler::s_mouseButton[GLFW_MOUSE_BUTTON_LAST + 1];
glm::dvec2 InputHandler::s_mousePosition = {0, 0};
glm::dvec2 InputHandler::s_mouseScroll = {0, 0};

bool InputHandler::isKeyPressed(uint16_t key) {
    static std::unordered_map<uint16_t, bool> previouslyPressed;
    bool currentlyPressed = s_keyButton[key];

    bool isPressedEvent = !previouslyPressed[key] && currentlyPressed;
    previouslyPressed[key] = currentlyPressed;

    return isPressedEvent;
}

bool InputHandler::isMousePressed(uint8_t key) {
    static std::unordered_map<uint16_t, bool> previouslyPressed;
    bool currentlyPressed = s_mouseButton[key];

    bool isPressedEvent = !previouslyPressed[key] && currentlyPressed;
    previouslyPressed[key] = currentlyPressed;

    return isPressedEvent;
}

bool InputHandler::isKeyHeld(uint16_t key) {
    return s_keyButton[key];
}

bool InputHandler::isMouseHeld(uint16_t key) {
    return s_mouseButton[key];
}

const glm::dvec2 &InputHandler::getMousePosition() {
    return s_mousePosition;
}

const glm::dvec2 &InputHandler::getMouseScroll() {
    return s_mouseScroll;
}

void InputHandler::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        s_keyButton[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        s_keyButton[key] = false;
    }
}

void InputHandler::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        s_mouseButton[button] = true;
    }
    else if (action == GLFW_RELEASE) {
        s_mouseButton[button] = false;
    }
}

void InputHandler::cursorPositionCallback(GLFWwindow *window, double xpos, double ypos) {
    s_mousePosition.x = xpos;
    s_mousePosition.y = ypos;
}

void InputHandler::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    s_mouseScroll.x = xoffset;
    s_mouseScroll.y = yoffset;
}
