#pragma once

#include "Texture/Texture.hpp"
class Shader;

class LightOrigin {
public:
    LightOrigin(float height, float scale);
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

    void uploadObject(Type type, float height, float scale);
    void initTexture(Type type, const std::string &filepath);
};
