#include "Application.hpp"

Application::Application() {
    srand(static_cast<unsigned>(time(0)));

    m_window = Window::get();

    // Shaders
    m_worldShader = std::make_unique<Shader>("src/Shader/GLSL/Textured.vert", "src/Shader/GLSL/Textured.frag");
    m_lineShader = std::make_unique<Shader>("src/Shader/GLSL/Colored.vert", "src/Shader/GLSL/Colored.frag");
    m_interfaceShader = std::make_unique<Shader>("src/Shader/GLSL/Interface.vert", "src/Shader/GLSL/Textured.frag");

    // Load texture atlas
    Block::loadTextures();

    m_player = Player::get();
    m_world = World::get();

    m_player->init();
    m_world->sortChunks();

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::println("Maximum Vertex Attributes: {}", nrAttributes);

    m_worldShader->use();
    m_worldShader->setInt("texture0", 0);
}

void Application::mainLoop() {
    float lastFrame = 0.0f;
    float deltaTime = 0.0f;
    while (!m_window->shouldClose()) {
        update(deltaTime);
        render();

        m_window->pollEvents();
        m_window->swapBuffers();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }
}

void Application::update(float deltaTime) {
    if (InputHandler::isKeyPressed(GLFW_KEY_ESCAPE)) {
        m_window->setShouldClose(true);
    }

    if (InputHandler::isKeyPressed(GLFW_KEY_F1)) {
        m_wireFrameMode = !m_wireFrameMode;
    }

    m_player->movementInput(m_window, deltaTime);
}

void Application::render() {
    if (!m_wireFrameMode) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glClearColor(135 / 255.f, 206 / 255.f, 235 / 255.f, 1.f);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = m_player->getViewMat4();
    glm::mat4 projection = m_player->getProjectionMat4(m_window->getWidth(), m_window->getHeight());
    static glm::mat4 interfaceProjection = projection;

    m_worldShader->use();
    m_worldShader->setMat4("view", view);
    m_worldShader->setMat4("projection", projection);

    m_lineShader->use();
    m_lineShader->setMat4("view", view);
    m_lineShader->setMat4("projection", projection);

    m_interfaceShader->use();
    m_interfaceShader->setMat4("projection", interfaceProjection);

    // Render stuff here
    //-----------------------------------------------------------
    m_world->render(m_wireFrameMode, m_worldShader.get(), m_lineShader.get());
    m_player->drawCursor(m_wireFrameMode, m_interfaceShader.get());
}
