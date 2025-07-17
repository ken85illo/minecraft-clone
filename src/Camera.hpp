#pragma once

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <print>

class Camera {
public:
    Camera(float near,
    float far,
    glm::vec3 pos,
    float cameraSpeed,
    float sensitivity,
    float fov,
    uint16_t* windowWidth,
    uint16_t* windowHeight,
    float* deltaTime);

    GLFWcursorposfun getCursorCallback();
    GLFWscrollfun getScrollCallback();

    void onCursorMove(double xpos, double ypos);
    void onScroll(double xoffset, double yoffset);

    void moveFront();
    void moveBack();
    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();
    void speedUp();
    void speedDown();

    glm::mat4 getViewMat4();
    glm::mat4 getProjectionMat4();

private:
    const float m_cameraSpeed;
    float m_currentSpeed;
    float* const m_deltaTime;
    uint16_t *m_windowWidth, *m_windowHeight;
    float m_sensitivity, m_fov, m_near, m_far;

    glm::vec3 m_cameraPos, m_cameraFront, m_cameraUp;
    glm::vec3 m_frontVec;

    static void mouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};
