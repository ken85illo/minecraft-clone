#pragma once

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "Utils/Input/InputHandler.hpp"
#include <print>

class Window {
public:
    Window(uint16_t windowWidth, uint16_t windowHeight, const char* title, bool fullscreen);
    ~Window();

    // GLFW Functions
    void pollEvents();
    void swapBuffers();
    bool shouldClose() const;
    void setShouldClose(bool value);

    // Getter functions
    static Window* get();
    GLFWwindow* getWindow() const;
    uint16_t getWidth() const;
    uint16_t getHeight() const;

private:
    static Window* m_instance;
    const char* m_title;
    uint16_t m_windowWidth, m_windowHeight;
    GLFWwindow* m_window;

    void initWindow();
    void setupCallbacks();

    void setWindowSize(int windowWidth, int windowHeight);
    static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
};
