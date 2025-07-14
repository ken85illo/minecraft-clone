#include "Window.hpp"
#include <random>

Window* Window::instance = nullptr;
uint16_t Window::m_width = 0;
uint16_t Window::m_height = 0;

Window::Window(uint16_t width, uint16_t height, const char* title)
: m_title(title), m_deltaTime(0.0f) {
    m_width = width;
    m_height = height;

    srand(static_cast<unsigned>(time(0)));
    if(!glfwInit()) {
        std::println("Failed to initialize GLFW!");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    initWindow();

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::println("Maximum Vertex Attributes: {}", nrAttributes);
}

Window::~Window() {
    delete Camera::instance;
    delete m_shader;
    delete cubes;
    glfwTerminate();
}

void Window::initWindow() {
    m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);
    Camera::instance = new Camera(3.0f, 0.1f, &m_width, &m_height, &m_deltaTime);
    if(!m_window) {
        std::println("Failed to initialize window!");
        return;
    }
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, frameBufferSizeCallback);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(m_window, Camera::instance->getCallback());

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::println("Failed to initialize GLAD!");
        return;
    }

    glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    m_shader = new Shader("../src/shaders/shader.vert", "../src/shaders/shader.frag");

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

    m_shader->use();
    m_shader->setMat4("projection", projection);
    m_shader->setInt("texture0", 0);
}


void Window::mainLoop() {
    if(!m_window)
        return;

    float max_x = 20.0f, min_x = -20.0f;
    float max_y = 20.0f, min_y = -20.0f;
    float max_z = 20.0f, min_z = -20.0f;
    cubes = new Cube(500, m_shader, "../res/nerd_hd.jpg");
    for(int i = 0; i < cubes->getSize(); i++) {
        float x = min_x +
        static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max_x - min_x)));
        float y = min_y +
        static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max_y - min_y)));
        float z = min_z +
        static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max_z - min_z)));

        std::println("{} {} {}", x, y, z);
        cubes->setPos(i, x, y, z);
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
    if(glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);
    if(glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        Camera::instance->moveFront();
    if(glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        Camera::instance->moveBack();
    if(glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        Camera::instance->moveRight();
    if(glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        Camera::instance->moveLeft();

    glm::mat4 view = Camera::getViewMat4();
    m_shader->use();
    m_shader->setMat4("view", view);
}

void Window::render() {
    glClearColor(135 / 255.f, 206 / 255.f, 235 / 255.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cubes->render();
}

void Window::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    m_width = width;
    m_height = height;
}
