#pragma once

#include "Core/Player/Player.hpp"
#include "Core/Window.hpp"
#include "Shader/Shader.hpp"
#include "Terrain/World.hpp"
#include <GLFW/glfw3.h>

class Engine {
public:
    Engine();
    void mainLoop();

private:
    bool m_wireFrameMode = false;

    std::unique_ptr<Shader> m_worldShader;
    std::unique_ptr<Shader> m_lineShader;
    std::unique_ptr<Shader> m_interfaceShader;

    Window* m_window;
    std::unique_ptr<Player> m_player;
    std::unique_ptr<World> m_world;

    void render();
    void update(float deltaTime);
};
