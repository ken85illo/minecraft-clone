#pragma once

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

class Camera {
public:
    Camera(float near, float far, glm::vec3 pos, float cameraSpeed, float sensitivity, float fov);

    void onCursorMove(double xpos, double ypos);
    void onScroll(double xoffset, double yoffset);

    virtual void moveFront();
    virtual void moveBack();
    virtual void moveRight();
    virtual void moveLeft();
    virtual void moveUp();
    virtual void moveDown();
    void speedUp();
    void speedDown();

    glm::mat4 getViewMat4();
    glm::mat4 getProjectionMat4();

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
