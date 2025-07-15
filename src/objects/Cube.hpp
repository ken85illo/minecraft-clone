#pragma once

// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include "Atlas.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <vector>

enum Tex {
    DIRT_TEX = 0,
    GRASS_SIDE_TEX,
    GRASS_TOP_TEX,
};

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

    Cube(int size, Shader* shader, const char* texturePath, Block type);
    ~Cube();

    void render();
    void update();

    // Position modifications
    void setPos(uint32_t index, float x, float y, float z);
    void setPosX(uint32_t index, float x);
    void setPosY(uint32_t index, float y);
    void setPosZ(uint32_t index, float z);
    void move(uint32_t index, float x, float y, float z);

    // Scale modifications
    void setScale(uint32_t index, float x, float y, float z);
    void setScaleX(uint32_t index, float x);
    void setScaleY(uint32_t index, float y);
    void setScaleZ(uint32_t index, float z);
    void scale(uint32_t index, float x, float y, float z);

    // Rotation modifications
    void setRotation(uint32_t index, float x, float y, float z);
    void setRotationX(uint32_t index, float x);
    void setRotationY(uint32_t index, float y);
    void setRotationZ(uint32_t index, float z);
    void rotate(uint32_t index, float x, float y, float z);

    uint32_t size() const;
    Properties& get(uint32_t index) const;

private:
    uint32_t m_size;

    uint32_t *m_VAO, *m_VBO, *m_EBO;
    Shader* m_shader;
    Texture* m_texture;
    Properties* m_properties;
    Atlas* m_atlas;
};
