#pragma once

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "Atlas.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <vector>

enum Block {
    NONE = -1,
    DIRT_BLOCK = 0,
    GRASS_BLOCK,
};


class Cube {
public:
    struct Properties {
        float x;
        float y;
        float z;
        float width;
        float height;
        float depth;
        float rotX;
        float rotY;
        float rotZ;
    };

    Cube(Shader* shader, Texture* texture, uint32_t textureIndex, Block type);
    ~Cube();

    void render();
    void update();

    // Position modifications
    void setPos(float x, float y, float z);
    void setPosX(float x);
    void setPosY(float y);
    void setPosZ(float z);
    void move(float x, float y, float z);

    // Scale modifications
    void setScale(float x, float y, float z);
    void setScaleX(float x);
    void setScaleY(float y);
    void setScaleZ(float z);
    void scale(float x, float y, float z);

    // Rotation modifications
    void setRotation(float x, float y, float z);
    void setRotationX(float x);
    void setRotationY(float y);
    void setRotationZ(float z);
    void rotate(float x, float y, float z);

    uint32_t size() const;
    Properties getProperty() const;

private:
    enum Tex {
        DIRT_TEX = 0,
        GRASS_SIDE_TEX,
        GRASS_TOP_TEX,
    };

    uint32_t m_VAO, m_VBO, m_EBO;
    Properties m_properties;

    Shader* m_shader;
    Atlas* m_atlas;
    Texture* m_texture;
    uint32_t m_textureIndex;
};
