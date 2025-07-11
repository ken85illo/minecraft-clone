#include "Window.hpp"

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
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    delete tex;
    delete shader;
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

    shader = new Shader("../src/shaders/shader.vert", "../src/shaders/shader.frag");

    float vertices[] = {
        // rectangle
        // -0.5f, -0.5f, 0.0f, // bottom-left
        // -0.5f, 0.5f, 0.0f,  // top-left
        // 0.5f, 0.5f, 0.0f,   // top-right
        // 0.5f, -0.5f, 0.0f,  // bottom-right

        // vertex           //color         //tex coords
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f,  // top-left
        0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 2.0f,   // top-right
        0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 2.0f, 0.0f,  // bottom-right
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
    1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(
    2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    tex = new Texture(GL_TEXTURE_2D, 2);
    tex->setParameter(0, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    tex->setParameter(0, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    tex->setParameter(0, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    tex->setParameter(0, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    tex->loadImage(0, "../res/brick.jpg");

    tex->setParameter(1, GL_TEXTURE_WRAP_R, GL_REPEAT);
    tex->setParameter(1, GL_TEXTURE_WRAP_S, GL_REPEAT);
    tex->setParameter(1, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    tex->setParameter(1, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    tex->loadImage(1, "../res/dog.jpg");

    shader->use();
    shader->setInt("texture0", 0);
    shader->setInt("texture1", 1);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while(!glfwWindowShouldClose(m_window)) {
        update();
        render();

        glfwPollEvents();
        glfwSwapBuffers(m_window);
    }
}

void Window::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader->use();
    glBindVertexArray(VAO);
    tex->use(0);
    tex->use(1);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
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
