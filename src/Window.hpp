#pragma once


// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp"
#include "objects/Cube.hpp"
#include <cmath>
#include <print>
#include <stb_image.h>
#include <sys/types.h>

class Window {
public:
    static Window* instance; // singleton
    Camera* cam;

    Window(uint16_t width, uint16_t height, const char* title, GLFWmonitor* monitor);
    ~Window();
    void mainLoop();

    void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void onWindowResize(int width, int height);

private:
    uint16_t m_width, m_height;
    const char* m_title;
    GLFWwindow* m_window;
    Shader* m_shader;
    Texture* m_texture;
    float m_deltaTime;
    bool m_wireFrameMode;

    std::vector<Cube> chunk;

    void initWindow();
    void update();
    void render();

    static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
