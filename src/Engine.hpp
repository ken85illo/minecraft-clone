#pragma once

#include "core/Window.hpp"
#include "core/player/Player.hpp"
#include "shader/Shader.hpp"
#include "terrain/World.hpp"
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

    Window* m_window;
    Player* m_player;
    World* m_world;

    void render();
    void update(float deltaTime);
};
