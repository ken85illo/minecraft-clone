#include "Engine.hpp"

Engine::Engine() {
    srand(static_cast<unsigned>(time(0)));

    m_window = Window::get();
    m_worldShader =
    new Shader("../src/shader/glsl/world.vert", "../src/shader/glsl/world.frag");
    m_lineShader =
    new Shader("../src/shader/glsl/line.vert", "../src/shader/glsl/line.frag");
    m_player = new Player(m_lineShader);
    m_world = new World(m_player, m_worldShader);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::println("Maximum Vertex Attributes: {}", nrAttributes);
}

Engine::~Engine() {
    delete m_player;
    delete m_world;
    delete m_worldShader;
    delete m_lineShader;
}

void Engine::mainLoop() {
    float lastFrame = 0.0f;
    float deltaTime = 0.0f;
    while(!m_window->shouldClose()) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        update(deltaTime);
        render();

        m_window->pollEvents();
        m_window->swapBuffers();
    }
}

void Engine::update(float deltaTime) {
    // Player Controls
    // ---------------------------------------------------------------------------
    m_player->onCursorMove(InputHandler::getMousePosition().x,
    InputHandler::getMousePosition().y, m_window->getWidth(), m_window->getHeight());
    m_player->onScroll(
    InputHandler::getMouseScroll().x, InputHandler::getMouseScroll().y);

    if(InputHandler::isKeyHeld(GLFW_KEY_W))
        m_player->moveFront(deltaTime);
    if(InputHandler::isKeyHeld(GLFW_KEY_S))
        m_player->moveBack(deltaTime);
    if(InputHandler::isKeyHeld(GLFW_KEY_D))
        m_player->moveRight(deltaTime);
    if(InputHandler::isKeyHeld(GLFW_KEY_A))
        m_player->moveLeft(deltaTime);
    if(InputHandler::isKeyHeld(GLFW_KEY_SPACE))
        m_player->moveUp(deltaTime);
    if(InputHandler::isKeyHeld(GLFW_KEY_LEFT_CONTROL))
        m_player->moveDown(deltaTime);
    if(InputHandler::isMousePressed(GLFW_MOUSE_BUTTON_LEFT))
        m_player->destroyBlock();
    if(InputHandler::isMousePressed(GLFW_MOUSE_BUTTON_RIGHT))
        m_player->placeBlock();
    if(InputHandler::isKeyHeld(GLFW_KEY_LEFT_SHIFT))
        m_player->speedUp();
    else
        m_player->speedDown();
    // ---------------------------------------------------------------------------

    if(InputHandler::isKeyPressed(GLFW_KEY_ESCAPE))
        m_window->setShouldClose(true);

    if(InputHandler::isKeyPressed(GLFW_KEY_F1)) {
        m_wireFrameMode = !m_wireFrameMode;

        if(!m_wireFrameMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}

void Engine::render() {
    if(!m_wireFrameMode)
        glClearColor(135 / 255.f, 206 / 255.f, 235 / 255.f, 1.f);
    else
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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

    m_world->render();
    // m_player->drawRayLine();
}
