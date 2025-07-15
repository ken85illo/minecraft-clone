#include "Window.hpp"
#include <random>

Window* Window::instance = nullptr;

Window::Window(uint16_t width, uint16_t height, const char* title, GLFWmonitor* monitor)
: m_width(width), m_height(height), m_title(title), m_deltaTime(0.0f), m_wireFrameMode(false) {
    srand(static_cast<unsigned>(time(0)));

    m_window = glfwCreateWindow(m_width, m_height, m_title, monitor, nullptr);

    initWindow();

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::println("Maximum Vertex Attributes: {}", nrAttributes);
}

Window::~Window() {
    delete cam;
    delete m_shader;
    delete cubes;
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::initWindow() {
    cam = new Camera(3.0f, 0.1f, &m_width, &m_height, &m_deltaTime);
    if(!m_window) {
        std::println("Failed to initialize window!");
        return;
    }
    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, (void*)this);

    glfwSetFramebufferSizeCallback(m_window, frameBufferSizeCallback);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(m_window, cam->getCursorCallback());
    glfwSetScrollCallback(m_window, cam->getScrollCallback());
    glfwSetKeyCallback(m_window, keyCallback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::println("Failed to initialize GLAD!");
        return;
    }

    glEnable(GL_DEPTH_TEST);

    m_shader = new Shader("../src/shaders/shader.vert", "../src/shaders/shader.frag");
    m_shader->use();
    m_shader->setInt("texture0", 0);
}

void Window::mainLoop() {
    if(!m_window)
        return;

    // Spawn cubes in a random position
    // float max_x = 50.0f, min_x = -50.0f;
    // float max_y = 50.0f, min_y = -50.0f;
    // float max_z = 50.0f, min_z = -50.0f;
    // cubes = new Cube(1000, m_shader, "../res/nerd_hd.jpg");
    // for(int i = 0; i < cubes->size(); i++) {
    //     float x = min_x +
    //     static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max_x - min_x)));
    //     float y = min_y +
    //     static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max_y - min_y)));
    //     float z = min_z +
    //     static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max_z - min_z)));
    //
    //     std::println("{} {} {}", x, y, z);
    //     cubes->setPos(i, x, y, z);
    // }

    uint32_t rows = 16;
    uint32_t columns = 16;
    uint32_t depth = 16;
    cubes = new Cube(rows * columns * depth, m_shader, "../res/atlas.png", GRASS_BLOCK);

    for(int i = 0; i < rows; i++)
        for(int j = 0; j < columns; j++)
            for(int k = 0; k < depth; k++) {
                uint32_t index = i * columns * depth + j * depth + k;
                cubes->setPos(index, j, i, k - static_cast<float>(depth));
            }

    float lastFrame = 0.0f;
    while(!glfwWindowShouldClose(m_window)) {
        update();
        render();

        glfwPollEvents();
        glfwSwapBuffers(m_window);

        float currentFrame = glfwGetTime();
        m_deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }
}

void Window::update() {
    if(glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        cam->moveFront();
    if(glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        cam->moveBack();
    if(glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        cam->moveRight();
    if(glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        cam->moveLeft();
    if(glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cam->moveUp();
    if(glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        cam->moveDown();

    glm::mat4 view = cam->getViewMat4();
    glm::mat4 projection = cam->getProjectionMat4();

    m_shader->use();
    m_shader->setMat4("view", view);
    m_shader->setMat4("projection", projection);
}

void Window::render() {
    if(!m_wireFrameMode)
        glClearColor(135 / 255.f, 206 / 255.f, 235 / 255.f, 1.f);
    else
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cubes->render();
}

void Window::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Window* windowClass = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if(windowClass)
        windowClass->onWindowResize(width, height);
}

void Window::onWindowResize(int width, int height) {
    m_width = width;
    m_height = height;
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    static Window* windowClass = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if(windowClass)
        windowClass->onKeyCallback(window, key, scancode, action, mods);
}

void Window::onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        m_wireFrameMode = !m_wireFrameMode;

        if(!m_wireFrameMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}
