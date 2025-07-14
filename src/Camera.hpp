#pragma once

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <print>

class Camera {
public:
    static Camera* instance; // singleton
    static glm::mat4 getViewMat4();
    static glm::mat4 getProjectionMat4();

    Camera() = delete;
    Camera(float cameraSpeed, float sensitivity, uint16_t* windowWidth, uint16_t* windowHeight, float* deltaTime);
    GLFWcursorposfun getCursorCallback();
    GLFWscrollfun getScrollCallback();

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
    static float m_fov;

    static void mouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};
