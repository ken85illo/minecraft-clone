#pragma once

#include <glad/glad.h>
#include <print>
#include <stb_image.h>

typedef unsigned int uint;
typedef unsigned char uchar;

class Texture {
public:
    Texture(int target, int size);
    ~Texture();

    void loadImage(int index, const char* filepath);
    void setParameter(int index, int name, int value);
    void use(int index);

private:
    uint* m_ID;
    const int m_target;
    const int m_size;
};
