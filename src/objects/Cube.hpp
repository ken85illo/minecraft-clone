#pragma once

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "Shader.hpp"
#include "Texture.hpp"
#include <vector>

class Cube {
public:
    struct Properties {
        float x;
        float y;
        float z;
        float width;
        float height;
        float depth;
    };

    Cube(int size, Shader* shader, const char* texturePath);
    ~Cube();

    void render();
    void update();

    void setPos(uint32_t index, float x, float y, float z);
    void setPosX(uint32_t index, float x);
    void setPosY(uint32_t index, float y);
    void setPosZ(uint32_t index, float z);
    void move(uint32_t index, float x, float y, float z);

    void setScale(uint32_t index, float x, float y, float z);
    uint32_t getSize() const;

private:
    uint32_t m_size;

    uint32_t *m_VAO, *m_VBO, *m_EBO;
    Shader* m_shader;
    Texture* m_texture;
    Properties* m_properties;
};
