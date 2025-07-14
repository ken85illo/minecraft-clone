#pragma once

#include <glad/glad.h>
#include <print>
#include <stb_image.h>
#include <sys/types.h>


class Texture {
public:
    Texture(int target, int size);
    ~Texture();

    void loadImage(const char* filepath);
    void bind(int index);
    void unbind(int index);
    void setParameter(int name, int value);

private:
    uint* m_ID;
    const int m_target;
    const int m_size;
};
