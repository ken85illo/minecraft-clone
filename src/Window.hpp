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

    Window(uint16_t width, uint16_t height, const char* title);
    ~Window();
    void mainLoop();

private:
    static glm::vec3 m_cameraPos, m_cameraFront, m_cameraUp;
    static uint16_t m_width, m_height;
    const char* m_title;
    GLFWwindow* m_window;
    Shader* m_shader;
    float m_deltaTime;
    Cube* cubes;

    void initWindow();
    void update();
    void render();

    static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
};
