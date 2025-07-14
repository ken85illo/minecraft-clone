#include "Camera.hpp"

Camera* Camera::instance = nullptr;
glm::vec3 Camera::m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 Camera::m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 Camera::m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

uint16_t* Camera::m_windowWidth = nullptr;
uint16_t* Camera::m_windowHeight = nullptr;
float Camera::m_sensitivity = 0.0f;

Camera::Camera(float m_cameraSpeed,
float sensitivity,
uint16_t* windowWidth,
uint16_t* windowHeight,
float* deltaTime)
: m_cameraSpeed(m_cameraSpeed), m_deltaTime(deltaTime) {
    m_windowWidth = windowWidth;
    m_windowHeight = windowHeight;
    m_sensitivity = sensitivity;

    m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::moveFront() {
    float speed = *m_deltaTime * m_cameraSpeed;
    m_cameraPos += m_cameraFront * speed;
}

void Camera::moveBack() {
    float speed = *m_deltaTime * m_cameraSpeed;
    m_cameraPos -= m_cameraFront * speed;
}

void Camera::moveRight() {
    float speed = *m_deltaTime * m_cameraSpeed;
    m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * speed;
}

void Camera::moveLeft() {
    float speed = *m_deltaTime * m_cameraSpeed;
    m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_cameraUp)) * speed;
}

glm::mat4 Camera::getViewMat4() {
    return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

void Camera::mouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    static bool firstMouse = true;
    static float lastX = *m_windowWidth / 2.0f;
    static float lastY = *m_windowHeight / 2.0f;

    if(firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    std::println("{} {}", xpos, ypos);

    xoffset *= m_sensitivity;
    yoffset *= m_sensitivity;

    static float pitch = 0.0f;
    static float yaw = -90.0f;

    pitch += yoffset;
    yaw += xoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    m_cameraFront = glm::normalize(direction);
}

GLFWcursorposfun Camera::getCallback() {
    return mouseCursorPosCallback;
}
