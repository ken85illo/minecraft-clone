#include "Camera.hpp"
#include "Window.hpp"

Camera::Camera(float near,
float far,
glm::vec3 pos,
float cameraSpeed,
float sensitivity,
float fov,
uint16_t* windowWidth,
uint16_t* windowHeight,
float* deltaTime)
: m_near(near),
  m_far(far),
  m_cameraSpeed(cameraSpeed),
  m_currentSpeed(cameraSpeed),
  m_sensitivity(sensitivity),
  m_windowWidth(windowWidth),
  m_windowHeight(windowHeight),
  m_deltaTime(deltaTime),
  m_fov(fov),
  m_cameraPos(pos),
  m_cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
  m_cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
  m_frontVec(glm::vec3(0.0f, 0.0f, -1.0f)) {
}

void Camera::moveFront() {
    float speed = *m_deltaTime * m_currentSpeed;
    m_cameraPos += m_frontVec * speed;
}

void Camera::moveBack() {
    float speed = *m_deltaTime * m_currentSpeed;
    m_cameraPos -= m_frontVec * speed;
}

void Camera::moveRight() {
    float speed = *m_deltaTime * m_currentSpeed;
    m_cameraPos += glm::normalize(glm::cross(m_frontVec, m_cameraUp)) * speed;
}

void Camera::moveLeft() {
    float speed = *m_deltaTime * m_currentSpeed;
    m_cameraPos -= glm::normalize(glm::cross(m_frontVec, m_cameraUp)) * speed;
}

void Camera::moveUp() {
    float speed = *m_deltaTime * m_currentSpeed;
    m_cameraPos += m_cameraUp * speed;
}

void Camera::moveDown() {
    float speed = *m_deltaTime * m_currentSpeed;
    m_cameraPos -= m_cameraUp * speed;
}

void Camera::speedUp() {
    m_currentSpeed = m_cameraSpeed * 2;
}

void Camera::speedDown() {
    m_currentSpeed = m_cameraSpeed;
}

glm::mat4 Camera::getViewMat4() {
    glm::vec3 dir = glm::normalize(m_cameraPos - (m_cameraPos + m_cameraFront));
    glm::vec3 right = glm::normalize(glm::cross(m_cameraUp, dir));
    glm::vec3 up = glm::cross(dir, right);

    glm::mat4 first = glm::mat4(1.0f);
    first[0] = glm::vec4(right.x, up.x, dir.x, 0.0f);
    first[1] = glm::vec4(right.y, up.y, dir.y, 0.0f);
    first[2] = glm::vec4(right.z, up.z, dir.z, 0.0f);

    glm::mat4 second = glm::mat4(1.0f);
    second[3] = glm::vec4(-m_cameraPos, 1.0f);

    // same implementation as glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp)
    // just experimenting for a bit lol (its part of an exercise in learnopengl)
    return first * second;
}

glm::mat4 Camera::getProjectionMat4() {
    return glm::perspective(
    glm::radians(m_fov), *m_windowWidth / (float)*m_windowHeight, m_near, m_far);
}

void Camera::mouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    static Window* windowClass = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if(windowClass && windowClass->cam)
        windowClass->cam->onCursorMove(xpos, ypos);
}

void Camera::onCursorMove(double xpos, double ypos) {
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
    // std::println("{} {}", xpos, ypos);

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

    glm::vec3 directionXZ;
    directionXZ.x = cos(glm::radians(yaw));
    directionXZ.z = sin(glm::radians(yaw));
    m_frontVec = glm::normalize(directionXZ);

    glm::vec3 directionXYZ;
    directionXYZ.x = directionXZ.x * cos(glm::radians(pitch));
    directionXYZ.y = sin(glm::radians(pitch));
    directionXYZ.z = directionXZ.z * cos(glm::radians(pitch));
    m_cameraFront = glm::normalize(directionXYZ);
}

void Camera::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    static Window* windowClass = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if(windowClass && windowClass->cam)
        windowClass->cam->onScroll(xoffset, yoffset);
}

void Camera::onScroll(double xoffset, double yoffset) {
    m_fov -= (float)yoffset;

    if(m_fov < 1.0f)
        m_fov = 1.0f;
    if(m_fov > 45.0f)
        m_fov = 45.0f;
}

GLFWcursorposfun Camera::getCursorCallback() {
    return mouseCursorPosCallback;
}

GLFWscrollfun Camera::getScrollCallback() {
    return scrollCallback;
}
