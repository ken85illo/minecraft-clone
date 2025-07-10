#include "Window.hpp"
#include <print>

Window* Window::instance = nullptr;


Window::Window(uint16_t width, uint16_t height, const char* title)
: m_width(width), m_height(height), m_title(title) {

    if(!glfwInit()) {
        std::println("Failed to initialize GLFW!");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    initWindow();

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::println("Maximum Vertex Attributes: {}", nrAttributes);
}

Window::~Window() {
    glfwTerminate();
}

void Window::initWindow() {
    m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
    if(!m_window) {
        std::println("Failed to initialize window!");
        return;
    }
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, frameBufferSizeCallback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        std::println("Failed to initialize GLAD!");
}


void Window::mainLoop() {
    if(!m_window)
        return;

    shader1 = new Shader();
    shader2 = new Shader();

    uint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    uint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    uint fragmentShader0 = glCreateShader(GL_FRAGMENT_SHADER);

    shader1->loadShader("../src/shader.vert", vertexShader);
    shader1->loadShader("../src/shader.frag", fragmentShader);
    shader1->linkProgram();

    shader2->loadShader("../src/shader.vert", vertexShader);
    shader2->loadShader("../src/shader0.frag", fragmentShader0);
    shader2->linkProgram();

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader0);

    float vertices[] = {
        // rectangle
        // -0.5f, -0.5f, 0.0f, // bottom-left
        // -0.5f, 0.5f, 0.0f,  // top-left
        // 0.5f, 0.5f, 0.0f,   // top-right
        // 0.5f, -0.5f, 0.0f,  // bottom-right

        // first triangle
        -0.95f, -0.5f, 0.0f, //
        -0.5f, 0.5f, 0.0f,   //
        -0.05f, -0.5f, 0.0f, //

    };

    float vertices1[] = {
        // second triangle
        0.05f, -0.5f, 0.0f, //
        0.5f, 0.5f, 0.0f,   //
        0.95f, -0.5f, 0.0f  //
    };

    uint indices[] = {
        0, 1, 2, // first triangle
        0, 2, 3, // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glBindVertexArray(VAO1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while(!glfwWindowShouldClose(m_window)) {
        update();
        render();

        glfwPollEvents();
        glfwSwapBuffers(m_window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader1->getProgram());
    glDeleteProgram(shader2->getProgram());
}

void Window::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader1->getProgram());
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glUseProgram(shader2->getProgram());
    glBindVertexArray(VAO1);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void Window::update() {
    if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window, true);
    }
}

void Window::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
