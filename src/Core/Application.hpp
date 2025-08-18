#pragma once

#include "Core/Window.hpp"
#include "Player/Player.hpp"
#include "Shader/Shader.hpp"
#include "World/World.hpp"
#include <GLFW/glfw3.h>

class Application {
  public:
    Application();
    void mainLoop();

  private:
    bool m_wireFrameMode = false;

    std::unique_ptr<Shader> m_worldShader;
    std::unique_ptr<Shader> m_lineShader;
    std::unique_ptr<Shader> m_interfaceShader;

    Window *m_window;
    World *m_world;
    Player *m_player;

    void render();
    void update(float deltaTime);
};
