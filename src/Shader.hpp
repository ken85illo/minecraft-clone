#pragma once

#include <fstream>
#include <glad/glad.h>
#include <print>
#include <string>

typedef unsigned int uint;

class Shader {
public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    void use();

    // Uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

private:
    uint m_ID;
};
