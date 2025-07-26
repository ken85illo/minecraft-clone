#include "Window.hpp"

Window* Window::instance = nullptr;

Window::Window(const char* title, GLFWmonitor* monitor)
: m_title(title), m_wireFrameMode(false) {

    srand(static_cast<unsigned>(time(0)));
    m_window = glfwCreateWindow(
    Engine::windowWidth, Engine::windowHeight, m_title, monitor, nullptr);
    initWindow();

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::println("Maximum Vertex Attributes: {}", nrAttributes);
}

Window::~Window() {
    delete m_world;
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::initWindow() {
    if(!m_window) {
        std::println("Failed to initialize window!");
        return;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, (void*)this);

    glfwSetFramebufferSizeCallback(m_window, frameBufferSizeCallback);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(m_window, mouseCursorPosCallback);
    glfwSetScrollCallback(m_window, scrollCallback);
    glfwSetKeyCallback(m_window, keyCallback);
    glfwSetMouseButtonCallback(m_window, mouseButtonCallback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::println("Failed to initialize GLAD!");
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    Engine::worldShader =
    new Shader("../src/shaders/world.vert", "../src/shaders/world.frag");
    Engine::lineShader = new Shader("../src/shaders/line.vert", "../src/shaders/line.frag");
}

void Window::mainLoop() {
    if(!m_window)
        return;

    m_player = new Player();
    m_world = new World(m_player);

    float lastFrame = 0.0f;
    while(!glfwWindowShouldClose(m_window)) {
        float currentFrame = glfwGetTime();
        Engine::deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        update();
        render();

        glfwPollEvents();
        glfwSwapBuffers(m_window);
    }
}

void Window::update() {
    if(glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        m_player->moveFront();
    if(glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        m_player->moveBack();
    if(glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        m_player->moveRight();
    if(glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        m_player->moveLeft();
    if(glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_player->moveUp();
    if(glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        m_player->moveDown();
}

void Window::render() {
    if(!m_wireFrameMode)
        glClearColor(135 / 255.f, 206 / 255.f, 235 / 255.f, 1.f);
    else
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = m_player->getViewMat4();
    glm::mat4 projection = m_player->getProjectionMat4();

    Engine::worldShader->use();
    Engine::worldShader->setMat4("view", view);
    Engine::worldShader->setMat4("projection", projection);

    Engine::lineShader->use();
    Engine::lineShader->setMat4("view", view);
    Engine::lineShader->setMat4("projection", projection);

    m_world->render();
    // m_player->drawRayLine();
}

void Window::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    static Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if(self)
        self->onWindowResize(width, height);
}

void Window::onWindowResize(int width, int height) {
    Engine::windowWidth = width;
    Engine::windowHeight = height;
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    static Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if(self)
        self->onKeyCallback(window, key, scancode, action, mods);
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

    if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
        m_player->speedUp();
    if(key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
        m_player->speedDown();
}

void Window::mouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    static Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if(self && self->m_player)
        self->m_player->onCursorMove(xpos, ypos);
}


void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    static Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if(self && self->m_player)
        self->m_player->onScroll(xoffset, yoffset);
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    static Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if(self)
        self->onMouseButtonClicked(button, action, mods);
}

void Window::onMouseButtonClicked(int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        m_player->destroyBlock();
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        m_player->placeBlock();
}
