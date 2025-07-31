#include "Engine.hpp"

Engine::Engine() {
    srand(static_cast<unsigned>(time(0)));

    m_window = Window::get();
    m_worldShader =
    new Shader("../src/shader/glsl/world.vert", "../src/shader/glsl/world.frag");
    m_lineShader =
    new Shader("../src/shader/glsl/line.vert", "../src/shader/glsl/line.frag");
    m_player = new Player(m_lineShader);
    m_world = new World(m_player, m_worldShader, m_lineShader);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::println("Maximum Vertex Attributes: {}", nrAttributes);
}

Engine::~Engine() {
    delete m_player;
    delete m_world;
    delete m_worldShader;
    delete m_lineShader;
    delete m_window;
}

void Engine::mainLoop() {
    float lastFrame = 0.0f;
    float deltaTime = 0.0f;
    while(!m_window->shouldClose()) {
        update(deltaTime);
        render();

        m_window->pollEvents();
        m_window->swapBuffers();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }
}

void Engine::update(float deltaTime) {
    if(InputHandler::isKeyPressed(GLFW_KEY_ESCAPE))
        m_window->setShouldClose(true);

    if(InputHandler::isKeyPressed(GLFW_KEY_F1))
        m_wireFrameMode = !m_wireFrameMode;

    m_player->movementInput(m_window, deltaTime);
}

void Engine::render() {
    if(!m_wireFrameMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glClearColor(135 / 255.f, 206 / 255.f, 235 / 255.f, 1.f);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = m_player->getViewMat4();
    glm::mat4 projection =
    m_player->getProjectionMat4(m_window->getWidth(), m_window->getHeight());

    m_worldShader->use();
    m_worldShader->setMat4("view", view);
    m_worldShader->setMat4("projection", projection);

    m_lineShader->use();
    m_lineShader->setMat4("view", view);
    m_lineShader->setMat4("projection", projection);

    m_world->render(m_wireFrameMode);
    // m_player->drawRayLine();
}
