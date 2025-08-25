#pragma once

#include "Texture/Texture.hpp"
class Shader;

class LightOrigin {
public:
    LightOrigin(float speed, float height, float scale);
    ~LightOrigin();
    void render(Shader *shader);

    glm::vec3 getLightPosition();

private:
    enum class Type : uint8_t {
        SUN,
        MOON,
    };

    uint32_t m_VAO[2], m_VBO[2], m_EBO[2];
    std::unique_ptr<Texture> m_texture[2];
    glm::vec4 m_position;
    float m_speed;
    float m_height;
    float m_scale;

    void uploadObject(Type type);
    void initTexture(Type type, const std::string &filepath);
};
