#pragma once

#include "Core/Window.hpp"

class Camera {
public:
    Camera(float near, float far, glm::vec3 pos, float cameraSpeed, float sensitivity, float fov);

    void onCursorMove(double xpos, double ypos, uint16_t windowWidth, uint16_t windowHeight);
    void onScroll(double xoffset, double yoffset);

    void moveFront(float deltaTime);
    void moveBack(float deltaTime);
    void moveRight(float deltaTime);
    void moveLeft(float deltaTime);
    void moveUp(float deltaTime);
    void moveDown(float deltaTime);

    void speedUp();
    void speedDown();

    glm::vec3 getPosition() const;
    glm::vec3 getFront() const;
    glm::mat4 getViewMat4() const;
    glm::mat4 getProjectionMat4(uint16_t windowWidth, uint16_t windowHeight) const;

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
