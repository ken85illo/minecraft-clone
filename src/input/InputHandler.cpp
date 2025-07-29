#include "InputHandler.hpp"

bool InputHandler::keyButton[GLFW_KEY_LAST + 1];
bool InputHandler::mouseButton[GLFW_MOUSE_BUTTON_LAST + 1];
MousePosition InputHandler::mousePosition = { 0, 0 };
MouseScroll InputHandler::mouseScroll = { 0, 0 };

bool InputHandler::isKeyPressed(uint16_t key) {
    static std::unordered_map<uint16_t, bool> previouslyPressed;
    bool currentlyPressed = keyButton[key];

    bool isPressedEvent = !previouslyPressed[key] && currentlyPressed;
    previouslyPressed[key] = currentlyPressed;

    return isPressedEvent;
}

bool InputHandler::isMousePressed(uint8_t key) {
    static std::unordered_map<uint16_t, bool> previouslyPressed;
    bool currentlyPressed = mouseButton[key];

    bool isPressedEvent = !previouslyPressed[key] && currentlyPressed;
    previouslyPressed[key] = currentlyPressed;

    return isPressedEvent;
}

bool InputHandler::isKeyHeld(uint16_t key) {
    return keyButton[key];
}

bool InputHandler::isMouseHeld(uint16_t key) {
    return mouseButton[key];
}

const MousePosition& InputHandler::getMousePosition() {
    return mousePosition;
}

const MouseScroll& InputHandler::getMouseScroll() {
    return mouseScroll;
}

void InputHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action == GLFW_PRESS)
        keyButton[key] = true;
    else if(action == GLFW_RELEASE)
        keyButton[key] = false;
}

void InputHandler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if(action == GLFW_PRESS)
        mouseButton[button] = true;
    else if(action == GLFW_RELEASE)
        mouseButton[button] = false;
}

void InputHandler::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    mousePosition.x = xpos;
    mousePosition.y = ypos;
}

void InputHandler::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    mouseScroll.x = xoffset;
    mouseScroll.y = yoffset;
}
