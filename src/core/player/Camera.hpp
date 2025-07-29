#pragma once

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "core/Window.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

class Camera {
public:
    Camera(float near, float far, glm::vec3 pos, float cameraSpeed, float sensitivity, float fov);

    void onCursorMove(double xpos, double ypos, uint16_t windowWidth, uint16_t windowHeight);
    void onScroll(double xoffset, double yoffset);

    virtual void moveFront(float deltaTime);
    virtual void moveBack(float deltaTime);
    virtual void moveRight(float deltaTime);
    virtual void moveLeft(float deltaTime);
    virtual void moveUp(float deltaTime);
    virtual void moveDown(float deltaTime);
    virtual void movementInput(Window* window, float deltaTime);
    void speedUp();
    void speedDown();

    glm::mat4 getViewMat4();
    glm::mat4 getProjectionMat4(uint16_t windowWidth, uint16_t windowHeight);

protected:
    glm::vec3 m_pos, m_front, m_up;
    glm::vec3 m_frontXZ;
    float m_pitch, m_yaw;
    float m_currentSpeed;

private:
    float m_sensitivity, m_fov, m_currentFov, m_near, m_far;
    float m_lastX, m_lastY;

    const float m_cameraSpeed;
};
