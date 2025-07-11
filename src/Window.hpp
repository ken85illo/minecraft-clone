#pragma once

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "Shader.hpp"
#include "Texture.hpp"
#include <cmath>
#include <print>
#include <stb_image.h>

typedef unsigned int uint;
typedef unsigned char uchar;

class Window {
public:
    static Window* instance; // singleton

    Window(uint16_t width, uint16_t height, const char* title);
    ~Window();
    void mainLoop();

private:
    const uint16_t m_width, m_height;
    const char* m_title;
    GLFWwindow* m_window;

    // Vertex and Fragment Shader
    Shader* shader;
    Texture* tex;
    uint VBO, VAO, EBO;


    void initWindow();
    void render();
    void update();

    static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
};
