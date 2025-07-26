#pragma once

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "World.hpp"
#include <stb_image.h>
#include <sys/types.h>

class Window {
public:
    static Window* instance; // singleton

    Window(const char* title, GLFWmonitor* monitor);
    ~Window();
    void mainLoop();

    void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void onWindowResize(int width, int height);
    void onMouseButtonClicked(int button, int action, int mods);

private:
    const char* m_title;
    GLFWwindow* m_window;
    bool m_wireFrameMode;

    Player* m_player;
    World* m_world;

    void initWindow();
    void update();
    void render();

    static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
};
