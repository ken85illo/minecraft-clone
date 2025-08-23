#include "LightOrigin.hpp"
#include "Player/Player.hpp"
#include "Shader/Shader.hpp"

LightOrigin::LightOrigin(float speed, float height, float scale)
: m_position(0.0f, height, 0.0f, 1.0f), m_speed(speed), m_height(height), m_scale(scale) {
    glGenVertexArrays(2, m_VAO);
    glGenBuffers(2, m_VBO);
    glGenBuffers(2, m_EBO);

    uploadObject(Type::SUN);
    initTexture(Type::SUN, "res/sunlight.png");

    uploadObject(Type::MOON);
    initTexture(Type::MOON, "res/moonlight.png");
}

void LightOrigin::uploadObject(Type type) {
    float vertices[20] = {
        -0.5f * m_scale, m_height, -0.5f * m_scale, 0.0f, 0.0f, // bottom-left
        0.5f * m_scale,  m_height, -0.5f * m_scale, 1.0f, 0.0f, // bottom-right
        0.5f * m_scale,  m_height, 0.5f * m_scale,  1.0f, 1.0f, // top-right
        -0.5f * m_scale, m_height, 0.5f * m_scale,  0.0f, 1.0f, // top-left
    };

    uint8_t indices[6] = {
        0, 1, 3, // first triangle
        1, 2, 3, // second triangle
    };

    uint8_t index = static_cast<size_t>(type);
    glBindVertexArray(m_VAO[index]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO[index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO[index]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void LightOrigin::initTexture(Type type, const std::string &filepath) {
    uint8_t index = static_cast<size_t>(type);
    m_texture[index] = std::make_unique<Texture>(GL_TEXTURE_2D, 1);
    m_texture[index]->bind(0);
    m_texture[index]->setParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    m_texture[index]->setParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    m_texture[index]->loadImage(filepath.c_str());
}

glm::vec3 LightOrigin::getLightPosition() {
    return glm::vec3(-m_position.x, m_position.y, m_position.z);
}

void LightOrigin::render(Shader *shader) {
    glm::vec3 playerPos = Player::get()->getPosition();
    glm::mat4 model = glm::mat4(1.0f);

    shader->use();
    model = glm::translate(model, glm::vec3(playerPos.x, MAX_HEIGHT / 2.0f, playerPos.z));
    model = glm::rotate(model, glm::radians(static_cast<float>(glfwGetTime() * m_speed)), glm::vec3(0.0f, 0.0f, 1.0f));
    static glm::vec4 originalPos = m_position;
    m_position = originalPos * model;

    shader->setMat4("model", model);
    m_texture[static_cast<size_t>(Type::SUN)]->bind(0);
    glBindVertexArray(m_VAO[static_cast<size_t>(Type::SUN)]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);

    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    shader->setMat4("model", model);
    m_texture[static_cast<size_t>(Type::MOON)]->bind(0);
    glBindVertexArray(m_VAO[static_cast<size_t>(Type::MOON)]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, nullptr);
}
