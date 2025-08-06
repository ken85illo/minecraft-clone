#pragma once

#include "Core/Player/Player.hpp"
#include "Core/Window.hpp"
#include "Shader/Shader.hpp"
#include "Terrain/World.hpp"
#include <GLFW/glfw3.h>

class Engine {
public:
    Engine();
    ~Engine();
    void mainLoop();

private:
    bool m_wireFrameMode = false;
    Shader* m_worldShader;
    Shader* m_lineShader;
    Shader* m_interfaceShader;

    Window* m_window;
    Player* m_player;
    World* m_world;

    void render();
    void update(float deltaTime);
};
