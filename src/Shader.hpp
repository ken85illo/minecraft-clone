#pragma once

#include <glad/glad.h>

typedef unsigned int uint;

class Shader {
public:
    Shader() : m_program(glCreateProgram()) {
    }

    void loadShader(const char* filepath, uint shader);
    uint getProgram() const;
    void linkProgram() const;

private:
    uint m_program;
};
