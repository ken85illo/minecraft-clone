#pragma once

#include "glm/ext/matrix_transform.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <print>

class Camera {
public:
    static Camera* instance; // singleton
    static glm::mat4 getViewMat4();

    Camera() = delete;
    Camera(float cameraSpeed, float sensitivity, uint16_t* windowWidth, uint16_t* windowHeight, float* deltaTime);
    GLFWcursorposfun getCallback();

    void moveFront();
    void moveBack();
    void moveRight();
    void moveLeft();

private:
    const float m_cameraSpeed;
    float* const m_deltaTime;
    static uint16_t *m_windowWidth, *m_windowHeight;
    static float m_sensitivity;
    static glm::vec3 m_cameraPos, m_cameraFront, m_cameraUp;

    static void mouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
};
