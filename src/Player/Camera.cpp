#include "Camera.hpp"

Camera::Camera(float near, float far, glm::vec3 pos, float cameraSpeed, float sensitivity, float fov)
: m_near(near),
  m_far(far),
  m_cameraSpeed(cameraSpeed),
  m_currentSpeed(cameraSpeed),
  m_sensitivity(sensitivity),
  m_fov(fov),
  m_currentFov(fov),
  m_pos(pos),
  m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
  m_up(glm::vec3(0.0f, 1.0f, 0.0f)),
  m_frontXZ(glm::vec3(0.0f, 0.0f, -1.0f)),
  m_pitch(0.0f),
  m_yaw(-90.0f) {}

void Camera::moveFront(float deltaTime) {
    float speed = deltaTime * m_currentSpeed;
    m_pos += m_frontXZ * speed;
}

void Camera::moveBack(float deltaTime) {
    float speed = deltaTime * m_currentSpeed;
    m_pos -= m_frontXZ * speed;
}

void Camera::moveRight(float deltaTime) {
    float speed = deltaTime * m_currentSpeed;
    m_pos += glm::normalize(glm::cross(m_frontXZ, m_up)) * speed;
}

void Camera::moveLeft(float deltaTime) {
    float speed = deltaTime * m_currentSpeed;
    m_pos -= glm::normalize(glm::cross(m_frontXZ, m_up)) * speed;
}

void Camera::moveUp(float deltaTime) {
    float speed = deltaTime * m_currentSpeed;
    m_pos += m_up * speed;
}

void Camera::moveDown(float deltaTime) {
    float speed = deltaTime * m_currentSpeed;
    m_pos -= m_up * speed;
}

void Camera::speedUp() {
    m_currentSpeed = m_cameraSpeed * 2;
}

void Camera::speedDown() {
    m_currentSpeed = m_cameraSpeed;
}

glm::vec3 Camera::getPosition() const {
    return m_pos;
}

glm::vec3 Camera::getFront() const {
    return m_front;
}

glm::mat4 Camera::getViewMat4() const {
    glm::vec3 dir = glm::normalize(-m_front);
    glm::vec3 right = glm::normalize(glm::cross(m_up, dir));
    glm::vec3 up = glm::cross(dir, right);

    glm::mat4 first = glm::mat4(1.0f);
    first[0] = glm::vec4(right.x, up.x, dir.x, 0.0f);
    first[1] = glm::vec4(right.y, up.y, dir.y, 0.0f);
    first[2] = glm::vec4(right.z, up.z, dir.z, 0.0f);

    glm::mat4 second = glm::mat4(1.0f);
    second[3] = glm::vec4(-m_pos, 1.0f);

    // same implementation as glm::lookAt(m_pos, -m_front, m_up)
    // just experimenting for a bit lol (its part of an exercise in learnopengl)
    return first * second;
}

glm::mat4 Camera::getProjectionMat4(uint16_t windowWidth, uint16_t windowHeight) const {
    return glm::perspective(glm::radians(m_currentFov), windowWidth / (float)windowHeight, m_near, m_far);
}

void Camera::onCursorMove(double xpos, double ypos, uint16_t windowWidth, uint16_t windowHeight) {
    static bool firstMouse = true;
    static double lastX = windowWidth / 2.0;
    static double lastY = windowHeight / 2.0;

    if (firstMouse) {
        m_lastX = xpos;
        m_lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - m_lastX;
    float yoffset = m_lastY - ypos;
    m_lastX = xpos;
    m_lastY = ypos;
    // std::println("{} {}", xpos, ypos);

    xoffset *= m_sensitivity;
    yoffset *= m_sensitivity;

    m_pitch += yoffset;
    m_yaw += xoffset;

    if (m_pitch > 89.0f) {
        m_pitch = 89.0f;
    }
    if (m_pitch < -89.0f) {
        m_pitch = -89.0f;
    }

    glm::vec3 directionXZ;
    directionXZ.x = cos(glm::radians(m_yaw));
    directionXZ.z = sin(glm::radians(m_yaw));
    m_frontXZ = glm::normalize(directionXZ);

    glm::vec3 directionXYZ;
    directionXYZ.x = directionXZ.x * cos(glm::radians(m_pitch));
    directionXYZ.y = sin(glm::radians(m_pitch));
    directionXYZ.z = directionXZ.z * cos(glm::radians(m_pitch));
    m_front = glm::normalize(directionXYZ);
}

void Camera::onScroll(double xoffset, double yoffset) {
    m_currentFov -= yoffset;

    if (m_currentFov < 20.0f) {
        m_currentFov = 20.0f;
    }
    if (m_currentFov > m_fov) {
        m_currentFov = m_fov;
    }
}
