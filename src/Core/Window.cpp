#include "Window.hpp"

std::unique_ptr<Window> Window::s_instance;

Window::Window(uint16_t windowWidth, uint16_t windowHeight, const char* title, bool fullscreen)
: m_windowWidth(windowWidth), m_windowHeight(windowHeight), m_title(title) {

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    m_window = glfwCreateWindow(
    windowWidth, windowHeight, m_title, fullscreen ? monitor : nullptr, nullptr);

    if(!m_window) {
        std::println("Failed to initialize window!");
        return;
    }

    initWindow();
    setupCallbacks();
}

Window::~Window() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

Window* Window::get() {
    if(!s_instance)
        s_instance = std::make_unique<Window>(1920, 1080, "ain't no way bruh", true);

    return s_instance.get();
}

void Window::initWindow() {
    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, (void*)this);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::println("Failed to initialize GLAD!");
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::setupCallbacks() {
    glfwSetFramebufferSizeCallback(m_window, Window::frameBufferSizeCallback);
    glfwSetCursorPosCallback(m_window, InputHandler::cursorPositionCallback);
    glfwSetScrollCallback(m_window, InputHandler::scrollCallback);
    glfwSetMouseButtonCallback(m_window, InputHandler::mouseButtonCallback);
    glfwSetKeyCallback(m_window, InputHandler::keyCallback);
}


void Window::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    static Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if(self)
        self->setWindowSize(width, height);
}

void Window::setWindowSize(int windowWidth, int windowHeight) {
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
    glViewport(0, 0, windowWidth, windowHeight);
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::swapBuffers() {
    glfwSwapBuffers(m_window);
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Window::setShouldClose(bool value) {
    glfwSetWindowShouldClose(m_window, value);
}

GLFWwindow* Window::getWindow() const {
    return m_window;
}

uint16_t Window::getWidth() const {
    return m_windowWidth;
}
uint16_t Window::getHeight() const {
    return m_windowHeight;
}
